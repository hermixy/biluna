/*****************************************************************
 * $Id: acc_processtransdoc.cpp 2212 2015-01-28 16:45:01Z rutger $
 * Created: Mar 22, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_processtransdoc.h"

#include <cmath>
#include "acc.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_postgltransaction.h"
#include "acc_qachartmaster.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_ProcessTransDoc::ACC_ProcessTransDoc() {
    mTransDoc = NULL;
    mDebCredTransList = NULL;
    mGlTransList = NULL;
}

/**
 * Destructor
 */
ACC_ProcessTransDoc::~ACC_ProcessTransDoc() {
    RB_DEBUG->print("ACC_ProcessTransDoc::~ACC_ProcessTransDoc()");
}

/**
 * Process the (debtor/creditor) transaction document
 * @param td transaction document
 * @param dctList list of debtor/creditor (detail) transactions of document
 * @param glList (empty) list of GL transactions
 * return true on success
 */
bool ACC_ProcessTransDoc::execute(RB_ObjectBase* td, RB_ObjectContainer* dctList,
                                  double exchangeRate) {
    if (!td || !dctList) return false;

    mTransDoc = td;
    mDebCredTransList = dctList;

    if (exchangeRate != 1.0) {
        updateToCompanyCurrency(exchangeRate);
    }

    mGlTransList = new RB_ObjectContainer(ACC_MODELFACTORY->getRootId(), NULL,
                                          "ACC_GlTransList", ACC_OBJECTFACTORY);

    // Create the GL transactions list based on the sales/purchase invoice document
    if (!createGlTrans()) {
        return false;
    }

    // Start SQL transaction
    ACC_MODELFACTORY->getDatabase().transaction();

    // Post document (sales/purchase invoice)
    mTransDoc->dbUpdate(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);

    // Post debtor transactions (invoice items), these are not children of document!
    mDebCredTransList->dbUpdateList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne);

    // Post GL transactions
    mGlTransList->dbUpdateList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne);

    // Post transactions to ACC_GlSum and ACC_CostSum
    ACC_PostGlTransaction* oper =  new ACC_PostGlTransaction();
    RB_ObjectIterator* iter = mGlTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* glTrans = iter->currentObject();
        if (!oper->execute(glTrans)) {
            RB_DEBUG->error("ACC_ProcessTransDoc::execute() ERROR");
        }
    }

    // Commit SQL transaction
    ACC_MODELFACTORY->getDatabase().commit();

    // HACK: to make sure
    ACC_SqlCommonFunctions f;
    QVariant var = f.selectFromWhereId("id", "acc_transdoc", mTransDoc->getId());
    if (!var.isValid()) {
        mTransDoc->deleteFlag(RB2::FlagFromDatabase);
        mTransDoc->deleteFlag(RB2::FlagIsDeleted);
        mTransDoc->setFlag(RB2::FlagIsDirty);
        mTransDoc->dbUpdate(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
    }

    delete iter;
    delete oper;
    delete mGlTransList;
    mGlTransList = NULL;
    return true;
}

/**
 * Create new GL transactions in case user has edited the transactions
 * TODO in case amount is zero and only tax exists do not create glTrans
 * of zero for the amount but only for the tax, this is the case with
 * salesorder and complex taxation
 * @returns true on success
 */
bool ACC_ProcessTransDoc::createGlTrans() {
    double totalAmount = 0.0;
    double taxRev = 0.0;
    double totalDebit = 0.0;
    double totalCredit = 0.0; // negative amounts
    RB_ObjectBase* glTrans = NULL; // ACC_GlTrans

    if (mGlTransList->countMember() > 0) mGlTransList->erase();

    mRootId = ACC_MODELFACTORY->getRootId();
    mDate = mTransDoc->getValue("transdate").toString();
    mType = mTransDoc->getValue("doctype").toInt();
    mTypeNo = mTransDoc->getValue("transno").toInt();
    QDateTime dateTime = mTransDoc->getValue("transdate").toDateTime();
    mPeriod = dateTime.date().year() * 100 + dateTime.date().month();
    mTransDocId = mTransDoc->getId();

    setTransType(mType);

    RB_String docDescr = mTransDoc->getValue("description").toString();
    RB_String itemDescr = "";
    RB_String accountId = "";
    RB_String accountName = "";
    RB_String costCenterId = "";
    RB_String costCenterName = "";
    double amount = 0.0;


    RB_ObjectIterator* iter = mDebCredTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* dt = iter->currentObject();

        // add first GL transaction per item
        glTrans = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(), mGlTransList, "", true);

        // set parent id since no parent object of mGlTransList
        glTrans->setValue("parent", mRootId);
        glTrans->setValue("transdate", mDate);
        glTrans->setValue("type", mType);
        glTrans->setValue("transdocno", mTypeNo);
        glTrans->setValue("periodno", mPeriod);
        glTrans->setValue("transdoc_id", mTransDocId);
        glTrans->setValue("itemtrans_id", dt->getId());

        itemDescr = dt->getValue("description").toString();
        glTrans->setValue("description", itemDescr);
        accountId = dt->getValue("chartmaster_idx").toString();
        accountName = dt->getDValue("chartmaster_idx").toString();
        glTrans->setValue("chartmaster_idx", accountId);  // original data role value, ID
        glTrans->setDValue("chartmaster_idx", accountName);
        costCenterId = dt->getValue("costcenter_idx").toString();
        costCenterName = dt->getDValue("costcenter_idx").toString();
        glTrans->setValue("costcenter_idx", costCenterId);
        glTrans->setDValue("costcenter_idx", costCenterName);

        // accountcontrol
        RB_ObjectBase* aObj = ACC_QACHARTMASTER->getAcctObj(accountId);

        if (!aObj) {
            mGlTransList->erase();
            return false; // no data for this accountId
        }

        int itemAcctControl = aObj->getValue("accountcontrol").toInt();
        glTrans->setValue("accountcontrol", itemAcctControl);

        amount = dt->getValue("amount").toDouble();

        if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransBankCash) {
            glTrans->setValue("amount", -amount); // credit
        } else {
            glTrans->setValue("amount", amount); // debit
        }

        totalAmount += amount;

        // set allocation id and reference for bank transaction and totals,
        // (for gltrans directly done, no model in between)
        if (mTransType == ACC2::TransBankCash) {
            RB_String str;
            str = dt->getValue("transallocn_idx").toString();
            glTrans->setValue("transallocn_idx", str);
            str = dt->getDValue("transallocn_idx").toString();
            glTrans->setDValue("transallocn_idx", str);

            // totals for bank, debit/credit is separate for bank only
            if (amount >= 0.0) {
                totalDebit += amount;
            } else {
                totalCredit += amount; // negative amounts
            }
        }

        bool isTaxControlAccount = ACC_QACHARTMASTER->isTaxableAccount(accountId);

        // Sales taxes for regular debtor and creditor
        // with costcenter_idx
        // TODO: switch for without VAT? maybe not? 2012-07-09
        if ((mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor)
                                            && isTaxControlAccount) {
            amount = dt->getValue("taxhighamt").toDouble();
            RB_ObjectBase* glTrans;

            if (std::fabs(amount) > 0.005) {
                // not necessary:  && accountId != mSalesRevId && accountId != mPurchRevId) {
                if (mTransType == ACC2::TransDebtor) {
                    glTrans = createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccSalesTaxHighName(),
                                   ACC_QACHARTMASTER->getAccSalesTaxHighId(), -amount); // credit
                } else {
                    glTrans = createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccPurchaseTaxHighName(),
                                   ACC_QACHARTMASTER->getAccPurchaseTaxHighId(), amount); // debit
                }

                glTrans->setValue("costcenter_idx", costCenterId);
                glTrans->setDValue("costcenter_idx", costCenterName);
                totalAmount += amount;
            }

            amount = dt->getValue("taxlowamt").toDouble();

            if (std::fabs(amount) > 0.005) {
                if (mTransType == ACC2::TransDebtor) {
                    glTrans = createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccSalesTaxLowName(),
                                   ACC_QACHARTMASTER->getAccSalesTaxLowId(), -amount); // credit
                } else {
                    glTrans = createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccPurchaseTaxLowName(),
                                   ACC_QACHARTMASTER->getAccPurchaseTaxLowId(), amount); // debit
                }

                glTrans->setValue("costcenter_idx", costCenterId);
                glTrans->setDValue("costcenter_idx", costCenterName);
                totalAmount += amount;
            }

            amount = dt->getValue("taxotheramt").toDouble();

            if (std::fabs(amount) > 0.005) {
                if (mTransType == ACC2::TransDebtor) {
                    glTrans = createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccSalesTaxOtherName(),
                                   ACC_QACHARTMASTER->getAccSalesTaxOtherId(), -amount); // credit
                } else {
                    glTrans = createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccPurchaseTaxOtherName(),
                                   ACC_QACHARTMASTER->getAccPurchaseTaxOtherId(), amount); // debit
                }

                glTrans->setValue("costcenter_idx", costCenterId);
                glTrans->setDValue("costcenter_idx", costCenterName);
                totalAmount += amount;
            }
        } // end if regular debtor or creditor

        // Add transactions for reversed tax per item, based on taxotheramt
        // without costcenter_idx
        // if function 14 Sales reversed tax
        // or function 34 Purchase reversed tax
        // tax reversed
        if (itemAcctControl == ACC2::ControlSalesRevTax /*14*/
                || itemAcctControl == ACC2::ControlPurchaseRevTax /*34*/) {
            taxRev = dt->getValue("taxotheramt").toDouble();
        }

        if (itemAcctControl == ACC2::ControlSalesRevTax && std::fabs(taxRev) > 0.005) {
            // function 25 and 24
            createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxSalesPayRevName(),
                           ACC_QACHARTMASTER->getAccTaxSalesPayRevId(), -taxRev); // credit
            createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxSalesRecRevName(),
                           ACC_QACHARTMASTER->getAccTaxSalesRecRevId(), taxRev); // debit
        } else if (itemAcctControl == ACC2::ControlPurchaseRevTax && std::fabs(taxRev) > 0.005) {
            // function 45 and 44
            createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxPurchPayRevName(),
                           ACC_QACHARTMASTER->getAccTaxPurchPayRevId(), -taxRev); // credit
            createGlHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxPurchRecRevName(),
                           ACC_QACHARTMASTER->getAccTaxPurchRecRevId(), taxRev); // debit
        }

        taxRev = 0.0;
    } // for (iter)

    delete iter;

    // add corresponding transaction for bank/cash, debitor and creditor accountId
    // without costcenter_idx otherwise double
    if (mTransType == ACC2::TransBankCash) {
        createGlHelper(docDescr, ACC_QACHARTMASTER->getAccBankName(),
                       ACC_QACHARTMASTER->getAccBankId(), totalDebit); // debit separate
        createGlHelper(docDescr, ACC_QACHARTMASTER->getAccBankName(),
                       ACC_QACHARTMASTER->getAccBankId(), totalCredit); // credit separate
    } else if (mTransType == ACC2::TransDebtor) {
        createGlHelper(docDescr, ACC_QACHARTMASTER->getAccRecName(),
                       ACC_QACHARTMASTER->getAccRecId(), totalAmount); // debit
    } else if (mTransType == ACC2::TransCreditor) {
        createGlHelper(docDescr, ACC_QACHARTMASTER->getAccPayName(),
                       ACC_QACHARTMASTER->getAccPayId(), -totalAmount); // credit
    }

    return true;
}

/**
 * Create the transaction object and complete with data
 * @param descr description
 * @param acctName account name
 * @param acctId account ID
 * @param amount amount
 * @return ACC_GlTrans
 */
RB_ObjectBase* ACC_ProcessTransDoc::createGlHelper(const RB_String& descr,
        const RB_String& acctName, const RB_String& acctId, double amount) {
    RB_ObjectBase* glTrans
        = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(), mGlTransList, "", true);
    glTrans->setValue("parent", mRootId);
    glTrans->setValue("transdate", mDate);
    glTrans->setValue("type", mType);
    glTrans->setValue("transdocno", mTypeNo);
    glTrans->setValue("periodno", mPeriod);
    glTrans->setValue("transdoc_id", mTransDocId);
    glTrans->setValue("description", descr);
    glTrans->setDValue("chartmaster_idx", acctName); // display role value
    glTrans->setValue("chartmaster_idx", acctId);  // original data role value
    // accountcontrol
    RB_ObjectBase* aObj = ACC_QACHARTMASTER->getAcctObj(acctId);
    if (aObj) {
        glTrans->setValue("accountcontrol", aObj->getValue("accountcontrol"));
    } else {
        glTrans->setValue("accountcontrol", 0);
    }
    glTrans->setValue("amount", amount); // debit

    return glTrans;
}

/**
 * Set transaction type (Debtor/Creditor or Memo) based on document type
 * @param docType transaction document type
 */
void ACC_ProcessTransDoc::setTransType(int docType) {
    switch (docType) {
    case ACC2::TransPurchOrder :
        mTransType = ACC2::TransCreditor;
        break;
    case ACC2::TransSalesOrder :
        mTransType = ACC2::TransDebtor;
        break;
    default:
        mTransType = ACC2::TransMemo;
    }
}

/**
 * Update amounts in transaction documents and detail transactions
 * to the currency of the company
 * @param exchangeRate exchange rate between company currency and
 * foreign currency
 */
void ACC_ProcessTransDoc::updateToCompanyCurrency(double exchangeRate) {
    if (!mTransDoc || !mDebCredTransList) {
        return;
    }

    mTransDoc->setValue("rate", exchangeRate);
    int transType = mTransDoc->getValue("doctype").toInt();
    double totalAmount = 0.0;
    double totalTax = 0.0;

    RB_ObjectIterator* iter = mDebCredTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        double val = obj->getValue("amount").toDouble() / exchangeRate;
        totalAmount += val;
        obj->setValue("amount", val);

        val = obj->getValue("taxhighamt").toDouble() / exchangeRate;
        totalTax += val;
        obj->setValue("taxhighamt", val);

        val = obj->getValue("taxlowamt").toDouble() / exchangeRate;
        totalTax += val;
        obj->setValue("taxlowamt", val);

        val = obj->getValue("taxotheramt").toDouble() / exchangeRate;
        totalTax += val;
        obj->setValue("taxotheramt", val);

        val = obj->getValue("unitprice").toDouble() / exchangeRate;
        obj->setValue("unitprice", val);
    }

    delete iter;

    if (transType == ACC2::TransSalesOrder) {
        mTransDoc->setValue("totalamountrec", totalAmount + totalTax);
    } else {
        mTransDoc->setValue("totalamountpay", totalAmount + totalTax);
    }

    mTransDoc->setValue("totaltax", totalTax);
}

