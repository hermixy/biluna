/*****************************************************************
 * $Id: acc_qachartmaster.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 26, 2011 5:34:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_qachartmaster.h"

#include "acc.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"
#include "acc_utilityfactory.h"


ACC_QaChartMaster* ACC_QaChartMaster::mActiveUtility = 0;


/**
 * Constructor
 */
ACC_QaChartMaster::ACC_QaChartMaster() : RB_Utility() {
    RB_DEBUG->print("ACC_QaChartMaster::ACC_QaChartMaster()");
    initMemberValues();
    ACC_UTILITYFACTORY->addUtility(this);
}

void ACC_QaChartMaster::initMemberValues() {
    mAccDefaultId = "";
    mAccDefaultName = "";

    mAccBankId = "";
    mAccBankName = "";
    mAccPayId = "";
    mAccPayName = "";
    mAccRecId = "";
    mAccRecName = "";

    mAccSalesTaxLowId = "";
    mAccSalesTaxLowName = "";
    mAccSalesTaxLowPercentage = 0.0;
    mAccSalesTaxHighId = "";
    mAccSalesTaxHighName = "";
    mAccSalesTaxHighPercentage = 0.0;
    mAccSalesTaxOtherId = "";
    mAccSalesTaxOtherName = "";
    mAccSalesTaxLowPercentage = 0.0;

    mAccPurchaseTaxLowId = "";
    mAccPurchaseTaxLowName = "";
    mAccPurchaseTaxLowPercentage = 0.0;
    mAccPurchaseTaxHighId = "";
    mAccPurchaseTaxHighName = "";
    mAccPurchaseTaxHighPercentage = 0.0;
    mAccPurchaseTaxOtherId = "";
    mAccPurchaseTaxOtherName = "";
    mAccPurchaseTaxOtherPercentage = 0.0;

    mAccSalesRevId = "";
    mAccSalesRevName = "";
    mAccPurchRevId = "";
    mAccPurchRevName = "";

    mAccTaxSalesPayRevId = "";
    mAccTaxSalesPayRevName = "";
    mAccTaxSalesRecRevId = "";
    mAccTaxSalesRecRevName = "";
    mAccTaxPurchPayRevId = "";
    mAccTaxPurchPayRevName = "";
    mAccTaxPurchRecRevId = "";
    mAccTaxPurchRecRevName = "";

    mFiscalMonthStart = 1;
    mFiscalDayStart = 1;
}

/**
 * Destructor
 */
ACC_QaChartMaster::~ACC_QaChartMaster() {
    ACC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = NULL;
    RB_DEBUG->print("ACC_QaChartMaster::~ACC_QaChartMaster() OK");
}

/**
 * Create or get instance
 * @return factory
 */
ACC_QaChartMaster* ACC_QaChartMaster::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new ACC_QaChartMaster();
        mActiveUtility->refresh();
    }
    return mActiveUtility;
}

/**
 * Get account object based on account ID
 * @param acctId account ID
 * @return account object
 */
RB_ObjectBase* ACC_QaChartMaster::getAcctObj(const RB_String& acctId) {
    RB_ObjectBase* acctObj = NULL;

    std::map<RB_String, RB_ObjectBase*>::iterator iterMap
            = mChartMasterMap.find(acctId);
    if (iterMap != mChartMasterMap.end()) {
        acctObj = iterMap->second;
    }

    return acctObj;
}

/**
 * @returns GL accounts/chartmaster list
 */
RB_ObjectContainer* ACC_QaChartMaster::getAcctList() {
    return mChartMasterList;
}

/**
 * Refresh GL accounts or chartmaster list,
 * called after changes to database table ACC_ChartMaster
 */
void ACC_QaChartMaster::refresh() {
    initMemberValues();
    setStandardAccounts();
    setCompanyDefaultTaxPercentages();
    setCompanyFiscalYearStart();
}

/**
 * Get the ID of the current GL default account
 * Used by system of no other account is available
 * set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @returns default account ID
 */
RB_String ACC_QaChartMaster::getAccDefaultId() {
    return mAccDefaultId;
}

/**
 * Set the ID of the GL default account
 * Used by system of no other account is available
 * only set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @param acctId ID
 */
void ACC_QaChartMaster::setAccDefaultId(const RB_String& acctId) {
    mAccDefaultId = acctId;
}

/**
 * Get the current name or description of the GL default account,
 * Used by system of no other account is available
 * set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @returns default account name
 */
RB_String ACC_QaChartMaster::getAccDefaultName() {
    return mAccDefaultName;
}

/**
 * Set the current name or description of the GL default account,
 * Used by system of no other account is available
 * only set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @param acctName account name or description
 */
void ACC_QaChartMaster::setAccDefaultName(const RB_String& acctName) {
    mAccDefaultName = acctName;
}

/**
 * Get the ID of the current GL bank account, only call when Ledger Account is Bank
 * set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @returns bank account ID
 */
RB_String ACC_QaChartMaster::getAccBankId() {
    return mAccBankId;
}

/**
 * Set the ID of the GL bank account
 * only set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @param acctId ID
 */
void ACC_QaChartMaster::setAccBankId(const RB_String& acctId) {
    mAccBankId = acctId;
}

/**
 * Get the current name or description of the GL bank account,
 * only call when Ledger Account is Bank
 * set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @returns bank account name
 */
RB_String ACC_QaChartMaster::getAccBankName() {
    return mAccBankName;
}

/**
 * Set the current name or description of the GL bank account,
 * only set after ACC_GlTransactionWidget::slotLedgerAccountChanged(int number)
 * @param acctName account name or description
 */
void ACC_QaChartMaster::setAccBankName(const RB_String& acctName) {
    mAccBankName = acctName;
}

/**
 * Get the ID of the payables GL account
 */
RB_String ACC_QaChartMaster::getAccPayId() {
    return mAccPayId;
}

/**
 * Get the name of the payables GL account
 */
RB_String ACC_QaChartMaster::getAccPayName() {
    return mAccPayName;
}

RB_String ACC_QaChartMaster::getAccDebtorAccruedId() {
    return mAccDebtorAccruedId;
}

RB_String ACC_QaChartMaster::getAccDebtorAccruedName() {
    return mAccDebtorAccruedName;
}

/**
 * Get the ID of the receivables GL account
 */
RB_String ACC_QaChartMaster::getAccRecId() {
    return mAccRecId;
}

/**
 * Get the name of the receivables GL account
 */
RB_String ACC_QaChartMaster::getAccRecName() {
    return mAccRecName;
}

RB_String ACC_QaChartMaster::getAccCreditorAccruedId() {
    return mAccCreditorAccruedId;
}

RB_String ACC_QaChartMaster::getAccCreditorAccruedName() {
    return mAccCreditorAccruedName;
}


/**
 * Get the ID of the sales tax low GL account
 * Company default only.
 */
RB_String ACC_QaChartMaster::getAccSalesTaxLowId() {
    return mAccSalesTaxLowId;
}

/**
 * Get the name of the sales tax low GL account
 */
RB_String ACC_QaChartMaster::getAccSalesTaxLowName() {
    return mAccSalesTaxLowName;
}

/**
 * Get the percentage of the sales tax low GL account
 */
double ACC_QaChartMaster::getAccSalesTaxLowPercentage() {
    return mAccSalesTaxLowPercentage;
}

/**
 * Get the ID of the sales tax high GL account
 */
RB_String ACC_QaChartMaster::getAccSalesTaxHighId() {
    return mAccSalesTaxHighId;
}

/**
 * Get the name of the sales tax high GL account
 */
RB_String ACC_QaChartMaster::getAccSalesTaxHighName() {
    return mAccSalesTaxHighName;
}

/**
 * Get the percentage of the sales tax high GL account
 */
double ACC_QaChartMaster::getAccSalesTaxHighPercentage() {
    return mAccSalesTaxHighPercentage;
}

/**
 * Get the ID of the sales tax other GL account
 */
RB_String ACC_QaChartMaster::getAccSalesTaxOtherId() {
    return mAccSalesTaxOtherId;
}

/**
 * Get the name of the sales tax other GL account
 */
RB_String ACC_QaChartMaster::getAccSalesTaxOtherName() {
    return mAccSalesTaxOtherName;
}

/**
 * Get the percentage of the sales tax other GL account
 */
double ACC_QaChartMaster::getAccSalesTaxOtherPercentage() {
    return mAccSalesTaxOtherPercentage;
}

/**
 * Get the ID of the purchase tax low GL account
 */
RB_String ACC_QaChartMaster::getAccPurchaseTaxLowId() {
    return mAccPurchaseTaxLowId;
}

/**
 * Get the name of the purchase tax low GL account
 */
RB_String ACC_QaChartMaster::getAccPurchaseTaxLowName() {
    return mAccPurchaseTaxLowName;
}

/**
 * Get the percentage of the purchase tax low GL account,
 * returns the same as getAccSalesTaxLowPercentage()
 */
double ACC_QaChartMaster::getAccPurchaseTaxLowPercentage() {
    return mAccPurchaseTaxLowPercentage;
}

/**
 * Get the ID of the purchase tax high GL account
 */
RB_String ACC_QaChartMaster::getAccPurchaseTaxHighId() {
    return mAccPurchaseTaxHighId;
}

/**
 * Get the name of the purchase tax high GL account
 */
RB_String ACC_QaChartMaster::getAccPurchaseTaxHighName() {
    return mAccPurchaseTaxHighName;
}

/**
 * Get the percentage of the purchase tax high GL account,
 * returns the same as getAccSalesTaxHighPercentage()
 */
double ACC_QaChartMaster::getAccPurchaseTaxHighPercentage() {
    return mAccPurchaseTaxHighPercentage;
}

/**
 * Get the ID of the purchase tax other GL account
 */
RB_String ACC_QaChartMaster::getAccPurchaseTaxOtherId() {
    return mAccPurchaseTaxOtherId;
}

/**
 * Get the name of the purchase tax other GL account
 */
RB_String ACC_QaChartMaster::getAccPurchaseTaxOtherName() {
    return mAccPurchaseTaxOtherName;
}

/**
 * Get the percentage of the purchase tax other GL account,
 * returns the same as getAccSalesTaxOtherPercentage()
 */
double ACC_QaChartMaster::getAccPurchaseTaxOtherPercentage() {
    return mAccPurchaseTaxOtherPercentage;
}

/**
 * Get the ID of the sales reversed GL account
 */
RB_String ACC_QaChartMaster::getAccSalesRevId() {
    return mAccSalesRevId;
}

/**
 * Get the name of the sales reversed GL account
 */
RB_String ACC_QaChartMaster::getAccSalesRevName() {
    return mAccSalesRevName;
}

/**
 * Get the ID of the purchase reversed GL account
 */
RB_String ACC_QaChartMaster::getAccPurchRevId() {
    return mAccPurchRevId;
}

/**
 * Get the name of the purchase reversed GL account
 */
RB_String ACC_QaChartMaster::getAccPurchRevName() {
    return mAccPurchRevName;
}

/**
 * Get the ID of the sales tax payable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxSalesPayRevId() {
    return mAccTaxSalesPayRevId;
}

/**
 * Get the name of the sales tax payable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxSalesPayRevName() {
    return mAccTaxSalesPayRevName;
}

/**
 * Get the ID of the sales tax receivable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxSalesRecRevId() {
    return mAccTaxSalesRecRevId;
}

/**
 * Get the name of the sales tax receivable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxSalesRecRevName() {
    return mAccTaxSalesRecRevName;
}

/**
 * Get the ID of the purchase tax payable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxPurchPayRevId() {
    return mAccTaxPurchPayRevId;
}

/**
 * Get the name of the purchase tax payable GL account
 */
RB_String ACC_QaChartMaster::getAccTaxPurchPayRevName() {
    return mAccTaxPurchPayRevName;
}

/**
 * Get the ID of the purchase tax receivable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxPurchRecRevId() {
    return mAccTaxPurchRecRevId;
}

/**
 * Get the name of the purchase tax receivable reversed GL account
 */
RB_String ACC_QaChartMaster::getAccTaxPurchRecRevName() {
    return mAccTaxPurchRecRevName;
}

/**
 * Get the ID of the memorandum GL account
 */
RB_String ACC_QaChartMaster::getAccMemoId() {
    return mAccMemoId;
}

/**
 * Get the name of the memorandum GL account
 */
RB_String ACC_QaChartMaster::getAccMemoName() {
    return mAccMemoName;
}

/**
 * Get the bank GL account ID based on the bank account number. This
 * requires the the bank GL account has the (full) number in the name
 * of the GL account of the chart master. This function is used
 * for the import of bank statements
 * @param bankGlAcctId bank account ID to be set
 * @param bankGlAcctName bank account name to be set
 * @param bankAccountNumber bank account number
 */
void ACC_QaChartMaster::getBankGlAcctIdName(RB_String& bankGlAcctId,
                                            RB_String& bankGlAcctName,
                                            const RB_String& bankAccountNumber) const {
    if (!mChartMasterList) {
        RB_DEBUG->error("ACC_QaChartMaster::getBankGlAccId() no mChartMasterList ERROR");
        return;
    }

    RB_ObjectIterator* iter = mChartMasterList->createIterator();

    for (iter->first(); !iter->isDone() && bankGlAcctId.isEmpty(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getValue("accountname").toString().contains(bankAccountNumber)) {
            bankGlAcctId = obj->getId();
            bankGlAcctName = obj->getValue("accountname").toString();
        }
    }

    delete iter;
}

void ACC_QaChartMaster::getAndClearMessageList(RB_StringList& messageList) {
    int count = mMessageList.size();

    for (int i = 0; i < count; ++i) {
        messageList.append(mMessageList.at(i));
    }

    mMessageList.clear();
}

QDate ACC_QaChartMaster::getFiscalYearStart(const QDate& currentDate) {
    QDate date;
    date.setDate(currentDate.year(), mFiscalMonthStart, mFiscalDayStart);

    if (date > currentDate) {
        date = date.addYears(-1);
    }

    return date;
}

QDate ACC_QaChartMaster::getFiscalYearEnd(const QDate& currentDate) {
    QDate date;
    date.setDate(currentDate.year(), mFiscalMonthStart, mFiscalDayStart);

    if (date <= currentDate) {
        date = date.addYears(1);
    }

    date = date.addDays(-1);
    return date;
}

bool ACC_QaChartMaster::isTaxableAccount(const RB_String& chartMasterId) {
    int itemAcctControl = -1;
    RB_ObjectIterator* iter = mChartMasterList->createIterator();
    iter->first();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getId() == chartMasterId) {
            itemAcctControl = obj->getValue("accountcontrol").toInt();
            iter->last();
        }
    }

    bool isTaxControl = false;

    // function 9, 11, 12, 13, 30, 31, 32, 33
    // function 61, 62, 63, 71, 72, 73
    if (itemAcctControl == ACC2::ControlSalesAllTax
            || itemAcctControl == ACC2::ControlSalesLowTax
            || itemAcctControl == ACC2::ControlSalesHighTax
            || itemAcctControl == ACC2::ControlSalesOtherTax
            || itemAcctControl == ACC2::ControlPurchaseAllTax
            || itemAcctControl == ACC2::ControlPurchaseLowTax
            || itemAcctControl == ACC2::ControlPurchaseHighTax
            || itemAcctControl == ACC2::ControlPurchaseOtherTax
            || itemAcctControl == ACC2::ControlPurchInEuLow
            || itemAcctControl == ACC2::ControlPurchInEuHigh
            || itemAcctControl == ACC2::ControlPurchInEuOther
            || itemAcctControl == ACC2::ControlPurchOutEuLow
            || itemAcctControl == ACC2::ControlPurchOutEuHigh
            || itemAcctControl == ACC2::ControlPurchOutEuOther) {
        isTaxControl = true;
    }

    return isTaxControl;
}

/**
 * Set the standard accounts such as Account Payable, -Receivable and
 * various tas ID and name for later use.
 * These values are frequently used when the user edits the accounting of
 * the sales- and purchase invoices.
 */
void ACC_QaChartMaster::setStandardAccounts() {
    mChartMasterList = ACC_MODELFACTORY->getRoot()->getContainer("ACC_ChartMasterList");
    mChartMasterList->erase();
    mChartMasterMap.clear();

    // Create the chartmaster (GL accounts) map
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String sqlStr = "SELECT * FROM acc_chartmaster WHERE ";
    sqlStr += "parent='" + ACC_MODELFACTORY->getRootId() + "' ORDER BY accountcode ASC;";
    query.exec(sqlStr);

    while (query.next()) {
        RB_ObjectBase* obj = ACC_OBJECTFACTORY->newObject("", mChartMasterList, "");
        obj->setValue("id", query.record().value(query.record().indexOf("id")));
        obj->setValue("parent", query.record().value(query.record().indexOf("parent")));
        obj->setValue("name", query.record().value(query.record().indexOf("name")));
        obj->setValue("status", query.record().value(query.record().indexOf("status")));
        obj->setValue("created", query.record().value(query.record().indexOf("created")));
        obj->setValue("changed", query.record().value(query.record().indexOf("changed")));
        obj->setValue("muser", query.record().value(query.record().indexOf("muser")));
        obj->setValue("accountcode", query.record().value(query.record().indexOf("accountcode")));
        obj->setValue("accountname", query.record().value(query.record().indexOf("accountname")));
        obj->setValue("accountgroup_id", query.record().value(query.record().indexOf("accountgroup_id")));
        obj->setValue("accountrequired", query.record().value(query.record().indexOf("accountrequired")));
        obj->setValue("accountcontrol", query.record().value(query.record().indexOf("accountcontrol")));
        mChartMasterMap[query.record().value(0).toString()]= obj;
    }

    std::map<RB_String, RB_ObjectBase*>::iterator iter;

    for (iter = mChartMasterMap.begin(); iter != mChartMasterMap.end(); ++iter) {
        RB_ObjectBase* actObj = iter->second;
        int actCtl = actObj->getValue("accountcontrol").toInt();

        switch (actCtl) {
        case ACC2::ControlDefault:
            mAccDefaultId = actObj->getId();
            mAccDefaultName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlDebtor :
            mAccRecId = actObj->getId();
            mAccRecName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlCreditor :
            mAccPayId = actObj->getId();
            mAccPayName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlDebtorAccrued :
            mAccDebtorAccruedId = actObj->getId();
            mAccDebtorAccruedName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlCreditorAccrued :
            mAccCreditorAccruedId = actObj->getId();
            mAccCreditorAccruedName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;

//        case ACC2::ControlTaxPayLow : // sales tax low TODO: should not be used anymore sales tax account is depending the authority
//            mAccSalesTaxLowId = actObj->getId();
//            mAccSalesTaxLowName = actObj->getValue("accountcode").toString()
//                    + " - " + actObj->getValue("accountname").toString();
//            break;
//        case ACC2::ControlTaxPayHigh : // sales tax high TODO: should not be used anymore sales tax ...
//            mAccSalesTaxHighId = actObj->getId();
//            mAccSalesTaxHighName = actObj->getValue("accountcode").toString()
//                    + " - " + actObj->getValue("accountname").toString();
//            break;
//        case ACC2::ControlTaxPayOther : // sales tax other TODO: should not be used anymore sales tax ...
//            mAccSalesTaxOtherId = actObj->getId();
//            mAccSalesTaxOtherName = actObj->getValue("accountcode").toString()
//                    + " - " + actObj->getValue("accountname").toString();
//            break;
        case ACC2::ControlTaxRecLow : // purchase tax low TODO: should not be used anymore purchase tax is depending the authority
            mAccPurchaseTaxLowId = actObj->getId();
            mAccPurchaseTaxLowName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlTaxRecHigh : // purchase tax high TODO: should not be used anymore purchase tax  ...
            mAccPurchaseTaxHighId = actObj->getId();
            mAccPurchaseTaxHighName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlTaxRecOther : // purchase tax other TODO: should not be used anymore purchase tax ...
            mAccPurchaseTaxOtherId = actObj->getId();
            mAccPurchaseTaxOtherName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlSalesRevTax : // Reversed Sales, function 14
            mAccSalesRevId = actObj->getId();
            mAccSalesRevName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlPurchaseRevTax : // Reversed Purchase, function 34
            mAccPurchRevId = actObj->getId();
            mAccPurchRevName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlTaxSalesPayRev : // Reversed tax sales, function 24
            mAccTaxSalesPayRevId = actObj->getId();
            mAccTaxSalesPayRevName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlTaxSalesRecRev : // Reversed tax sales, function 25
            mAccTaxSalesRecRevId = actObj->getId();
            mAccTaxSalesRecRevName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlTaxPurchPayRev : // Reversed tax sales, function 44
            mAccTaxPurchPayRevId = actObj->getId();
            mAccTaxPurchPayRevName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlTaxPurchRecRev : // Reversed tax sales, function 45
            mAccTaxPurchRecRevId = actObj->getId();
            mAccTaxPurchRecRevName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        case ACC2::ControlMemoBook :
            mAccMemoId = actObj->getId();
            mAccMemoName = actObj->getValue("accountcode").toString()
                    + " - " + actObj->getValue("accountname").toString();
            break;
        default :
            break;
        }
    }

    // If default account does not exist, set with first account from list
    RB_ObjectBase* obj = mChartMasterList->getObject("accountcontrol",
                                                     ACC2::ControlDefault);
    if (!obj) {
        RB_ObjectIterator* iter = mChartMasterList->createIterator();
        iter->first();
        RB_ObjectBase* actObj = iter->currentObject();
        mAccDefaultId = actObj->getId();
        mAccDefaultName = actObj->getValue("accountcode").toString()
                + " - " + actObj->getValue("accountname").toString();

    }
}

/**
 * Set company default tax percentages based on the default tax province
 * for ACC_GlTransWidget
 */
void ACC_QaChartMaster::setCompanyDefaultTaxPercentages() {
    RB_ObjectBase* company = ACC_MODELFACTORY->getRoot();
    RB_ObjectBase* actObj = NULL;
    int accountControl = 0;
    RB_String accountId = "";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    ACC_SqlCommonFunctions f;
    RB_String taxProvinceId = company->getValue("taxprovince_id").toString();
    RB_String taxGroupId = company->getValue("taxgroup_id").toString();
    f.getCompanyTaxes(query, taxProvinceId, taxGroupId);
    bool isError = false;

    mMessageList.clear();

    while (query.next()) {
        accountControl = query.value(0).toInt();
        accountId = query.value(1).toString();

        switch (accountControl) {
        case ACC2::ControlTaxPayLow :
            actObj = getAcctObj(accountId);
            if (actObj) {
                mAccSalesTaxLowId = accountId;
                mAccSalesTaxLowName = actObj->getValue("accountcode").toString()
                        + " - " + actObj->getValue("accountname").toString();
                mAccSalesTaxLowPercentage = query.value(2).toDouble();
                mAccPurchaseTaxLowPercentage = mAccSalesTaxLowPercentage;

                if (mAccSalesTaxLowId.isEmpty()) {
                    mMessageList.append("ACC_QaChartMaster::setCompanyDefaultTaxPercentages() WARNING:");
                    mMessageList.append(" mAccSalesTaxLowId is empty, check the following items, ");
                    mMessageList.append(" - Company tax province and tax group");
                    mMessageList.append(" - Tax authority and tax authority rates");
                    mMessageList.append(" - Sales (and/or purchase) tax postings");
                }
            } else {
                isError = true;
            }
            break;
        case ACC2::ControlTaxPayHigh :
            actObj = getAcctObj(accountId);
            if (actObj) {
                mAccSalesTaxHighId = accountId;
                mAccSalesTaxHighName = actObj->getValue("accountcode").toString()
                        + " - " + actObj->getValue("accountname").toString();
                mAccSalesTaxHighPercentage = query.value(2).toDouble();
                mAccPurchaseTaxHighPercentage = mAccSalesTaxHighPercentage;

                if (mAccSalesTaxHighId.isEmpty()) {
                    mMessageList.append("ACC_QaChartMaster::setCompanyDefaultTaxPercentages() WARNING:");
                    mMessageList.append(" mAccSalesTaxHighId is empty, check the following items, ");
                    mMessageList.append(" - Company tax province and tax group");
                    mMessageList.append(" - Tax authority and tax authority rates");
                    mMessageList.append(" - Sales (and/or purchase) tax postings");
                }
            } else {
                isError = true;
            }
            break;
        case ACC2::ControlTaxPayOther :
            actObj = getAcctObj(accountId);
            if (actObj) {
                mAccSalesTaxOtherId = accountId;
                mAccSalesTaxOtherName = actObj->getValue("accountcode").toString()
                        + " - " + actObj->getValue("accountname").toString();
                mAccSalesTaxOtherPercentage = query.value(2).toDouble();
                mAccPurchaseTaxOtherPercentage = mAccSalesTaxOtherPercentage;

                if (mAccSalesTaxOtherId.isEmpty()) {
                    mMessageList.append("ACC_QaChartMaster::setCompanyDefaultTaxPercentages() WARNING:");
                    mMessageList.append(" mAccSalesTaxOtherId is empty, check the following items, ");
                    mMessageList.append(" - Company tax province and tax group");
                    mMessageList.append(" - Tax authority and tax authority rates");
                    mMessageList.append(" - Sales (and/or purchase) tax postings");
                }
            } else {
                isError = true;
            }
            break;
        default :
            break;
        }
    }

    if (isError) {
        mMessageList.append("ACC_QaChartMaster::setCompanyDefaultTaxPercentages() ERROR:");
        mMessageList.append(" taxProvinceId = " + taxProvinceId);
        mMessageList.append(" taxGroupId = " + taxGroupId);
    }
}

void ACC_QaChartMaster::setCompanyFiscalYearStart() {
    RB_ObjectBase* company = ACC_MODELFACTORY->getRoot();
    mFiscalMonthStart = company->getValue("fiscalmonthstart").toInt();
    mFiscalDayStart = company->getValue("fiscaldaystart").toInt();
}
