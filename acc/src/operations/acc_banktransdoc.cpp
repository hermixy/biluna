/*****************************************************************
 * $Id: acc_banktransdoc.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Nov 23, 2012 5:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_banktransdoc.h"

#include "acc.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_qachartmaster.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_BankTransDoc::ACC_BankTransDoc() {
    mBankAcctId = "";
    mBankAcctName = "";
    mTotalAmountRec = 0.0;
    mTotalAmountPay = 0.0;
    mTransDate = QDate(1900,1,1);
}

/**
 * Destructor
 */
ACC_BankTransDoc::~ACC_BankTransDoc() {

}

/**
 * Execute the creation of bank documents (statements)
 * and respective line items.
 * @param transDocList bank transaction document list
 * @param biList bank import list
 * @return true on success
 */
bool ACC_BankTransDoc::execute(RB_ObjectBase* transDocList, RB_ObjectBase* biList) {
    if (!transDocList || !biList) return false;

    bool isValidBankAccount = false;
    mTransNo = -1;
    RB_ObjectBase* transDoc = NULL;
    RB_ObjectContainer* bankTransList = NULL;
    RB_ObjectBase* bankTrans = NULL;

    RB_ObjectIterator* iter = biList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* biObj = iter->currentObject();

        // check for bank account isValidBankAccount
        if (!isValidBankAccount) {
            RB_String bankAcctNo = biObj->getValue("statactnumber").toString();
            ACC_QACHARTMASTER->getBankGlAcctIdName(mBankAcctId, mBankAcctName, bankAcctNo);

            if (mBankAcctId != "") {
                isValidBankAccount = true;
            }

            // bank account does not exist return false do this check only once
            if (!isValidBankAccount) {
                return false;
            }
        }

        // set current statement number, set overall data and create transDoc
        int statNo = biObj->getValue("statnumber").toInt();
        if (mTransNo != statNo || !transDoc) {
            if (transDoc) {
                // Set data including mTotalAmountRec and mTotalAmountPay
                // Therefore not the first time when transDoc does not exist yet
                setTransDocData(transDoc);
                // Reset
                mTotalAmountRec = 0.0;
                mTotalAmountPay = 0.0;
                mDocDescription = "";
                mTransDate = biObj->getValue("stattimestamp").toDate();
            }

            mTransNo = statNo;

            transDoc = ACC_OBJECTFACTORY->newObject("", transDocList);
            // Add temporary list, because ACC_Project is parent of ACC_BankTrans
            bankTransList = new RB_ObjectContainer("", transDoc, "ACC_BankTransList",
                                          ACC_OBJECTFACTORY);
            transDoc->addObject(bankTransList);
        }

        QDate currentDate= biObj->getValue("transentrytimestamp").toDate();
        if (mTransDate  < currentDate) {
            mTransDate = currentDate;
            mDueDate = mTransDate;
        }

        // create new bank transaction item (bankTrans)
        bankTrans = ACC_OBJECTFACTORY->newObject("", bankTransList);
        setBankTransData(bankTrans, biObj);
        double amount = biObj->getValue("transamount").toDouble();

        if (amount >= 0.0) {
            mTotalAmountRec += amount;
        } else {
            mTotalAmountPay += -amount;
        }

        RB_DEBUG->print(biObj->getValue("transentrytimestamp").toString()
                        + " " + biObj->getValue("statnumber").toString()
                        + " " + biObj->getValue("transamount").toString());
    }

    // Last time
    setTransDocData(transDoc);

    return true;
}

/**
 * Set transaction document (Bank) with imported data
 * @param transDoc bank transaction statement
 * @param biObj bank import data object
 */
void ACC_BankTransDoc::setTransDocData(RB_ObjectBase* transDoc) {
    if (!transDoc) return;

    // set transaction document members
    transDoc->setValue("parent", mBankAcctId); // ACC_ChartMaster.id of bank
    transDoc->setValue("doctype", (int)ACC2::TransBankCash);
    transDoc->setValue("docname", mBankAcctName);
    transDoc->setValue("transdate", mTransDate);
    transDoc->setValue("duedate", mDueDate);
    transDoc->setValue("transno", mTransNo);
    transDoc->setValue("description", mDocDescription);
    transDoc->setValue("totalamountrec", mTotalAmountRec);
    transDoc->setValue("totalamountpay", mTotalAmountPay);
}

/**
 * Set bank transaction with imported data
 * @param bankTrans bank transaction
 * @param biObj bank import data object
 */
void ACC_BankTransDoc::setBankTransData(RB_ObjectBase* bankTrans,
                                        RB_ObjectBase* biObj){
    // set parent ID to ACC_Project, currently
    bankTrans->setValue("parent", ACC_MODELFACTORY->getRootId());
    bankTrans->setValue("transdoc_id", bankTrans->getGrandParent()->getId());
    RB_String itemDescr = biObj->getValue("transaccount").toString() + " "
            + biObj->getValue("transdescription").toString() + " "
            + biObj->getValue("transseconddescription").toString();
    appendTransDocDescription(itemDescr);
    bankTrans->setValue("description",itemDescr);
    bankTrans->setValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultId());
    bankTrans->setDValue("chartmaster_idx", ACC_QACHARTMASTER->getAccDefaultName());
//    bankTrans->setValue("accountcontrol", 0);
    bankTrans->setValue("amount", biObj->getValue("transamount").toDouble());
    bankTrans->setValue("type", (int)ACC2::TransBankCash);
    bankTrans->setValue("transno", biObj->getValue("statnumber"));
    bankTrans->setValue("bankaccountnumber", biObj->getValue("transactnumber"));
    bankTrans->setValue("amountcleared", 0.0);
//    bankTrans->setValue("exrate", 1.0);
//    bankTrans->setValue("functionalexrate", 1.0);
    bankTrans->setValue("transdate", biObj->getValue("transentrytimestamp")); // not stattimestamp
//    bankTrans->setValue("banktranstype", "");
    bankTrans->setValue("currency_id", "EUR");
    bankTrans->setValue("transallocn_idx", "0");

//    bool success = handleAllocation(bankTrans);

//    if (!success) {
//        handleGlAccount(bankTrans);
//    }
}

/**
 * Handle allocation if relevant
 * @param bankTrans bank transaction
 * @returns true if relevant debtor or credit booking was found
 */
//bool ACC_BankTransDoc::handleAllocation(RB_ObjectBase* bankTrans) {
//    // TODO: implement
//    return false;
//}

/**
 * Handle booking to account if relevant
 * @param bankTrans bank transaction
 * @returns true if user set GL account was found
 */
//bool ACC_BankTransDoc::handleGlAccount(RB_ObjectBase* bankTrans) {
//    // TODO: implement
//    return false;
//}

/**
 * Append part (15 letters) of the item description to
 * the transaction document description separated by comma
 * @param descr item description
 */
void ACC_BankTransDoc::appendTransDocDescription(const RB_String& descr) {
    if (mDocDescription.size() >= 255) {
        return;
    }

    if (!mDocDescription.isEmpty()) {
        mDocDescription += ",";
    }

    RB_String str = descr;
    str.truncate(15);
    mDocDescription += str;
}
