/*****************************************************************
 * $Id: acc_qasysacctcontrol.cpp 2202 2015-01-11 19:39:36Z rutger $
 * Created: Jan 18, 2012 5:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_qasysacctcontrol.h"

#include "acc.h"
#include "acc_sysacctcontrol.h"
#include "db_utilityfactory.h"


ACC_QaSysAcctControl* ACC_QaSysAcctControl::mActiveUtility = 0;


/**
 * Constructor
 */
ACC_QaSysAcctControl::ACC_QaSysAcctControl() : RB_UtilityFactory() {
    RB_DEBUG->print("ACC_QaSysAcctControl::ACC_QaSysAcctControl()");
    mRoot = NULL;
    DB_UTILITYFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
ACC_QaSysAcctControl::~ACC_QaSysAcctControl() {
    DB_UTILITYFACTORY->unregisterFactory(this);
    if (mRoot) {
        delete mRoot;
        mRoot = NULL;
    }
    mActiveUtility = NULL;
    RB_DEBUG->print("ACC_QaSysAcctControl::~ACC_QaSysAcctControl() OK");
}

/**
 * Create or get instance
 * @return factory
 */
ACC_QaSysAcctControl* ACC_QaSysAcctControl::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new ACC_QaSysAcctControl();
        mActiveUtility->setAccountControls();
    }
    return mActiveUtility;
}

/**
 * Get account object based on account ID
 * @param acctId account ID
 * @return account object
 */
RB_ObjectBase* ACC_QaSysAcctControl::getRoot() {
    return mRoot;

}

/**
 * Refresh  account control list
 */
void ACC_QaSysAcctControl::refresh() {
    setAccountControls();
}

int ACC_QaSysAcctControl::getTaxControlNumber(int accountControl) {
    if (!mRoot) {
        RB_DEBUG->error("ACC_QaSysAcctControl::getTaxControlNumber() root ERROR");
        return -1;
    }

    RB_ObjectContainer* objL = mRoot->getContainer("ACC_SysAcctControlList");
    RB_ObjectIterator* iter = objL->createIterator();
    int taxControl = -1;

    for (iter->first(); !iter->isDone() && taxControl < 0; iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (accountControl == obj->getValue("controlnumber").toInt()) {
            taxControl = obj->getValue("taxcontrolno").toInt();
        }
    }

    delete iter;
    return taxControl;
}

/**
 * Set account control list
 * @param itemList list of items
 */
void ACC_QaSysAcctControl::setAccountControlList(QStringList& itemList) {
    RB_ObjectContainer* objL = mRoot->getContainer("ACC_SysAcctControlList");
    RB_ObjectIterator* iter = objL->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        itemList << obj->getValue("description").toString();
    }

    delete iter;
}

/**
 * Set the standard accounts such as Account Payable, -Receivable and
 * various tas ID and name for later use.
 * These values are frequently used when the user edits the accounting of
 * the sales- and purchase invoices.
 */
void ACC_QaSysAcctControl::setAccountControls() {
    if (mRoot) {
        delete mRoot;
    }

    // create root object with container
    mRoot = new RB_ObjectContainer("", NULL, "ACC_SysAcctControlRoot");
    RB_ObjectContainer* list = new RB_ObjectContainer("", NULL, "ACC_SysAcctControlList");
    mRoot->addObject(list);

    addObject(list, ACC2::ControlDefault, "Default*", ACC2::ControlNone); // = 1, // 'Default', 0 (number other than 0 indicates the corresponding tax account)
    addObject(list, ACC2::ControlYearOpening, "Year Opening*", ACC2::ControlNone); // = 2, // 'jaar opening', 0
    addObject(list, ACC2::ControlSalesAllTax, "Sales All Tax* (VAT to 23)", ACC2::ControlTaxPayOther); // = 9, // 'verkopen/omzet alle btw tarieven', 23
    addObject(list, ACC2::ControlSalesNoTax, "Sales No Tax*", ACC2::ControlNone); // = 10, // 'verkopen/omzet onbelast (rubriek 1e)', 0
    addObject(list, ACC2::ControlSalesLowTax, "Sales Low Tax* (VAT to 21)", ACC2::ControlTaxPayLow); // = 11, // 'verkopen/omzet laag (excl btw: rubriek 1b)', 21
    addObject(list, ACC2::ControlSalesHighTax, "Sales High Tax* (VAT to 22)", ACC2::ControlTaxPayHigh); // = 12, // 'verkopen/omzet hoog (excl btw: rubriek 1a)', 22
    addObject(list, ACC2::ControlSalesOtherTax, "Sales Other Tax* (VAT to 23)", ACC2::ControlTaxPayOther); // = 13, // 'verkopen/omzet overig (excl btw: rubriek 1c)', 23
    addObject(list, ACC2::ControlSalesRevTax, "Sales Reversed Tax* (VAT to 24)", ACC2::ControlTaxSalesPayRev); // = 14, // 'verkopen/omzet verlegd (excl btw: rubriek 1e)', 24
    addObject(list, ACC2::ControlPrivateUseNoTax, "Private Use No Tax", ACC2::ControlNone); // = 16, // 'prive gebruik', 0
    addObject(list, ACC2::ControlPrivateUseTax, "Private Use Tax* (VAT to 29)", ACC2::ControlTaxPrivateUse); // = 19, // 'prive gebruik (excl btw: rubriek 1d einde jaar)', 29
    addObject(list, ACC2::ControlTaxPayLow, "Tax Payable Low* (VAT from 11)", ACC2::ControlNone); // = 21, // 'btw af te dragen laag (verkopen)', 0
    addObject(list, ACC2::ControlTaxPayHigh, "Tax Payable High* (VAT from 12)", ACC2::ControlNone); // = 22, // 'btw af te dragen hoog (verkopen)', 0
    addObject(list, ACC2::ControlTaxPayOther, "Tax Payable Other* (VAT from 9 and 13)", ACC2::ControlNone); // = 23, // 'btw af te dragen overig (verkopen)', 0
    addObject(list, ACC2::ControlTaxSalesPayRev, "Tax Sales Payable Reversed* (VAT from 14)", ACC2::ControlNone); // = 24, // 'btw af te dragen verlegd (verkopen)', 0
    addObject(list, ACC2::ControlTaxSalesRecRev, "Tax Sales Receivable Reversed", ACC2::ControlNone); // = 25, // 'btw te vorderen verlegd (verkopen)', 0
    addObject(list, ACC2::ControlTaxPayGlobLow, "Tax Payable Globalisation Low", ACC2::ControlNone); // = 26, // 'btw af te dragen globalisatie laag', 0
    addObject(list, ACC2::ControlTaxPayGlobHigh, "Tax Payable Globalistion High", ACC2::ControlNone); // = 27, // 'btw af te dragen globalisatie hoog', 0
    addObject(list, ACC2::ControlTaxPayGlobOther, "Tax Payable Globalisation Other", ACC2::ControlNone); // = 28, // 'btw af te dragen globalisatie overig', 0
    addObject(list, ACC2::ControlTaxPrivateUse, "Tax Payable Private Use*", ACC2::ControlNone); // = 29, // 'btw af te dragen prive gebruik', 0
    addObject(list, ACC2::ControlPurchaseAllTax, "Purchase All Tax* (VAT to 43)", ACC2::ControlTaxRecOther); // = 30, // 'inkopen/kosten alle btw tarieven (rubriek 5b)', 43
    addObject(list, ACC2::ControlPurchaseLowTax, "Purchase Low Tax* (VAT to 41)", ACC2::ControlTaxRecLow); // = 31, // 'inkopen/kosten laag (excl btw: rubriek 5b)', 41
    addObject(list, ACC2::ControlPurchaseHighTax, "Purchase High Tax* (VAT to 42)", ACC2::ControlTaxRecHigh); // = 32, // 'inkopen/kosten hoog (excl btw: rubriek 5b)', 42
    addObject(list, ACC2::ControlPurchaseOtherTax, "Purchase Other Tax* (VAT to 43)", ACC2::ControlTaxRecOther); // = 33, // 'inkopen/kosten overig (excl btw: rubriek 5b)', 43
    addObject(list, ACC2::ControlPurchaseRevTax, "Purchase Reversed Tax* (VAT to 44)", ACC2::ControlTaxPurchPayRev); // = 34, // 'inkopen/kosten verlegd (excl btw: rubriek 2a)', 44
    addObject(list, ACC2::ControlTaxRecLow, "Tax Receivable Low* (VAT from 31)", ACC2::ControlNone); // = 41, // 'btw te vorderen laag (inkopen)', 0
    addObject(list, ACC2::ControlTaxRecHigh, "Tax Receivable High* (VAT from 32)", ACC2::ControlNone); // = 42, // 'btw te vorderen hoog (inkopen)', 0
    addObject(list, ACC2::ControlTaxRecOther, "Tax Receivable Other* (VAT from 30 and 33)", ACC2::ControlNone); // = 43, // 'btw te vorderen overig (inkopen)', 0
    addObject(list, ACC2::ControlTaxPurchPayRev, "Tax Purchase Payable Reversed* (VAT from 34)", ACC2::ControlNone); // = 44, // 'btw af te dragen verlegd (inkopen)', 0
    addObject(list, ACC2::ControlTaxPurchRecRev, "Tax Purchase Receivable Reversed", ACC2::ControlNone); // = 45, // 'btw te vorderen verlegd (inkopen)', 0
    addObject(list, ACC2::ControlPurchInEuLow, "Purchase Inside EU Low* (VAT to 66)", ACC2::ControlTaxPurchInEu); // = 61, // 'inkopen import binnen EU laag (rubriek 4b)', 66
    addObject(list, ACC2::ControlPurchInEuHigh, "Purchase Inside EU High* (VAT to 66)", ACC2::ControlTaxPurchInEu); // = 62, // 'inkopen import binnen EU hoog (rubriek 4b)', 66
    addObject(list, ACC2::ControlPurchInEuOther, "Purchase Inside EU Other* (VAT to 66)", ACC2::ControlTaxPurchInEu); // = 63, // 'inkopen import binnen EU overig (rubriek 4b)', 66
    addObject(list, ACC2::ControlTaxPurchInEu, "Tax Purchase Receivable Inside EU* (VAT from 61,62,63)", ACC2::ControlNone); // = 66, // 'btw te vorderen inkopen import binnen EU (rubriek 4b)', 0
    addObject(list, ACC2::ControlPurchOutEuLow, "Purchase Outside EU Low* (VAT to 76)", ACC2::ControlTaxPurchOutEu); // = 71, // 'inkopen import buiten EU laag (rubriek 4a)', 76
    addObject(list, ACC2::ControlPurchOutEuHigh, "Purchase Outside EU High* (VAT to 76)", ACC2::ControlTaxPurchOutEu); // = 72, // 'inkopen import buiten EU hoog (rubriek 4a)', 76
    addObject(list, ACC2::ControlPurchOutEuOther, "Purchase Outside EU Other* (VAT to 76)", ACC2::ControlTaxPurchRecRev); // = 73, // 'inkopen import buiten EU overig (rubriek 4a)', 76
    addObject(list, ACC2::ControlTaxPurchOutEu, "Tax Purchase Receivable Outside EU* (VAT from 71,72,73)", ACC2::ControlNone); // = 76, // 'btw te vorderen inkopen import buiten EU (rubriek 4a)', 0
    addObject(list, ACC2::ControlSalesInEu, "Sales Inside EU*", ACC2::ControlNone); // = 80, // 'verkopen export binnen EU (rubriek 3b)', 0
    addObject(list, ACC2::ControlSalesOutEu, "Sales Outside EU*", ACC2::ControlNone); // = 81, // 'verkopen export buiten EU (rubriek 3a)', 0
    addObject(list, ACC2::ControlBadDebt, "Bad Debt", ACC2::ControlNone); // = 82, // 'oninbare vorderingen', 0
    addObject(list, ACC2::ControlCorrection, "Correction", ACC2::ControlNone); // = 83, // 'herrekening/correcties', 0
    addObject(list, ACC2::ControlInstallInEu, "Installation Inside EU", ACC2::ControlNone); // = 84, // 'installatie/televerkoop binnen EU', 0
    addObject(list, ACC2::ControlSalesBpm, "Sales BPM", ACC2::ControlNone); // = 85, // 'BPM verkoop', 0
    addObject(list, ACC2::ControlPurchBpm, "Purchase BPM", ACC2::ControlNone); // = 86, // 'BPM inkoop', 0
    addObject(list, ACC2::ControlStockBpm, "Stock BPM", ACC2::ControlNone); // = 87, // 'BPM voorraad', 0
    addObject(list, ACC2::ControlServiceInEu, "Service Inside EU*", ACC2::ControlNone); // = 88, // 'dienstverlening binnen EU (rubriek 3b)', 0
    addObject(list, ACC2::ControlSalesGlobLow, "Sales Globalisation Low*", ACC2::ControlTaxPayGlobLow); // = 91, // 'verkoop globalisatie laag (rubriek 1b)', 0
    addObject(list, ACC2::ControlSalesGlobHigh, "Sales Globalisation High*", ACC2::ControlTaxPayGlobHigh); // = 92, // 'verkoop globalisatie hoog (rubriek 1a)', 0
    addObject(list, ACC2::ControlSalesGlobOther, "Sales Globalisation Other*", ACC2::ControlTaxPayGlobOther); // = 93, // 'verkoop globalisatie overig (rubriek 1c)', 0
    addObject(list, ACC2::ControlPurchGlobLow, "Purchase Globalisation Low*", ACC2::ControlTaxPayGlobLow); // = 94, // 'inkoop globalisatie laag (rubriek 1b)', 0
    addObject(list, ACC2::ControlPurchGlobHigh, "Purchase Globalisation High*", ACC2::ControlTaxPayGlobHigh); // = 95, // 'inkoop globalisatie hoog (rubriek 1a)', 0
    addObject(list, ACC2::ControlPurchGlobOther, "Purchase Globalisation Other*", ACC2::ControlTaxPayGlobOther); // = 96, // 'inkoop globalisatie overig (rubriek 1c)', 0
    addObject(list, ACC2::ControlCreditLimitSales, "Credit Limit Sales", ACC2::ControlNone); // = 100, // 'kredietbeperking (verkopen)', 0
    addObject(list, ACC2::ControlCreditLimitPurch, "Credit Limit Purchase", ACC2::ControlNone); // = 101, // 'kredietbeperking (inkopen)', 0
    addObject(list, ACC2::ControlPayDiscount, "Payment Discount", ACC2::ControlNone); // = 102, // 'betalingskorting', 0
    addObject(list, ACC2::ControlSuspenseAccount, "Suspense Account", ACC2::ControlNone); // = 110, // 'tussenrekening betalingen', 0
    addObject(list, ACC2::ControlUnkownPayment, "Unknown Payment", ACC2::ControlNone); // = 111, // 'onbekende betalingen', 0
    addObject(list, ACC2::ControlCashPayment, "Cash Payment", ACC2::ControlNone); // = 120, // 'betaalwijze contant', 0
    addObject(list, ACC2::ControlElectronicPayment, "Electronic Payment", ACC2::ControlNone); // = 121, // 'betaalwijze electronisch', 0
    addObject(list, ACC2::ControlWorkCost, "Work Cost", ACC2::ControlNone); // = 185, // 'werkkosten regeling', 0
    addObject(list, ACC2::ControlDebtor, "Debtor Account*", ACC2::ControlNone); // = 1001, // 'Debiteuren (verkopen)', 0
    addObject(list, ACC2::ControlCreditor, "Creditor Account*", ACC2::ControlNone); // = 1002, // 'Crediteuren (inkopen)', 0
    addObject(list, ACC2::ControlDebtorAccrued, "Debtor Accrued*", ACC2::ControlNone); // 'Openstaande debiteuren (verkopen) van vorig boekjaar', 0
    addObject(list, ACC2::ControlCreditorAccrued, "Creditor Accrued*",ACC2::ControlNone);// 'Openstaande crediteuren (inkopen)van vorig boekjaar', 0
    addObject(list, ACC2::ControlCash, "Cash Register Account*", ACC2::ControlNone); // = 2000, // 'Kas', 0
    addObject(list, ACC2::ControlBank, "Bank Account*", ACC2::ControlNone); // = 2100, // 'Bank', 0
    addObject(list, ACC2::ControlMemoBook, "Memorandum Account*", ACC2::ControlNone); // = 3000, // 'Memoriaal', 0
    addObject(list, ACC2::ControlUserDefined, "User Defined", ACC2::ControlNone); // = 65536 // 2^16
    addObject(list, ACC2::ControlUserDefined1, "User Defined 1", ACC2::ControlNone); // = 65541, 'gebruiker gedefinieerd 1', 0
    addObject(list, ACC2::ControlUserDefined2, "User Defined 2", ACC2::ControlNone); // = 65542, 'gebruiker gedefinieerd 2', 0
    addObject(list, ACC2::ControlUserDefined3, "User Defined 3", ACC2::ControlNone); // = 65543, 'gebruiker gedefinieerd 3', 0
    addObject(list, ACC2::ControlUserDefined4, "User Defined 4", ACC2::ControlNone); // = 65544, 'gebruiker gedefinieerd 4', 0
    addObject(list, ACC2::ControlUserDefined5, "User Defined 5", ACC2::ControlNone); // = 65545, 'gebruiker gedefinieerd 5', 0
    addObject(list, ACC2::ControlUserDefined6, "User Defined 6", ACC2::ControlNone); // = 65546, 'gebruiker gedefinieerd 6', 0
    addObject(list, ACC2::ControlUserDefined7, "User Defined 7", ACC2::ControlNone); // = 65547, 'gebruiker gedefinieerd 7', 0
    addObject(list, ACC2::ControlUserDefined8, "User Defined 8", ACC2::ControlNone); // = 65548, 'gebruiker gedefinieerd 8', 0
    addObject(list, ACC2::ControlUserDefined9, "User Defined 9", ACC2::ControlNone); // = 65549, 'gebruiker gedefinieerd 9', 0
}

void ACC_QaSysAcctControl::addObject(RB_ObjectContainer* list, int acctControl,
                                     const RB_String& description, int taxControl) {

    ACC_SysAcctControl* obj = new ACC_SysAcctControl("", list, "ACC_SysAcctControl");

    obj->setValue("controlnumber", acctControl);
    obj->setValue("description", description);
    obj->setValue("taxcontrolno", taxControl);
    list->addObject(obj);
}
