/*****************************************************************
 * $Id: acc_qachartmaster.h 2202 2015-01-11 19:39:36Z rutger $
 * Created: Jan 26, 2011 5:34:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_QACHARTMASTER_H
#define ACC_QACHARTMASTER_H

//#include <iostream>
//#include <string>

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"
#include "rb_string.h"
#include "rb_utility.h"


#define ACC_QACHARTMASTER ACC_QaChartMaster::getInstance()


/**
 * Quick Access to chartmaster Instance (static) class,
 * this class is instantiated once and can be accessed with ACC_QACHARTMASTER
 * The chartmaster is queried many times for different purposes,
 * this class is used for example during processing of invoices and for
 * general accounting/booking of invoices
 * Utililities are handled independently with utility factory
 */
class ACC_QaChartMaster : public RB_Utility {

public:
    virtual ~ACC_QaChartMaster();
    static ACC_QaChartMaster* getInstance();

    RB_ObjectBase* getAcctObj(const RB_String& acctId);
    RB_ObjectContainer* getAcctList();
    void refresh();

    RB_String getAccDefaultId();
    void setAccDefaultId(const RB_String& acctId);
    RB_String getAccDefaultName();
    void setAccDefaultName(const RB_String& acctName);

    RB_String getAccBankId();
    void setAccBankId(const RB_String& acctId);
    RB_String getAccBankName();
    void setAccBankName(const RB_String& acctName);

    RB_String getAccPayId();
    RB_String getAccPayName();
    RB_String getAccDebtorAccruedId();
    RB_String getAccDebtorAccruedName();
    RB_String getAccRecId();
    RB_String getAccRecName();
    RB_String getAccCreditorAccruedId();
    RB_String getAccCreditorAccruedName();

    RB_String getAccSalesTaxLowId();
    RB_String getAccSalesTaxLowName();
    double getAccSalesTaxLowPercentage();
    RB_String getAccSalesTaxHighId();
    RB_String getAccSalesTaxHighName();
    double getAccSalesTaxHighPercentage();
    RB_String getAccSalesTaxOtherId();
    RB_String getAccSalesTaxOtherName();
    double getAccSalesTaxOtherPercentage();

    RB_String getAccPurchaseTaxLowId();
    RB_String getAccPurchaseTaxLowName();
    double getAccPurchaseTaxLowPercentage();
    RB_String getAccPurchaseTaxHighId();
    RB_String getAccPurchaseTaxHighName();
    double getAccPurchaseTaxHighPercentage();
    RB_String getAccPurchaseTaxOtherId();
    RB_String getAccPurchaseTaxOtherName();
    double getAccPurchaseTaxOtherPercentage();

    RB_String getAccSalesRevId();
    RB_String getAccSalesRevName();
    RB_String getAccPurchRevId();
    RB_String getAccPurchRevName();
    RB_String getAccTaxSalesPayRevId();
    RB_String getAccTaxSalesPayRevName();
    RB_String getAccTaxSalesRecRevId();
    RB_String getAccTaxSalesRecRevName();
    RB_String getAccTaxPurchPayRevId();
    RB_String getAccTaxPurchPayRevName();
    RB_String getAccTaxPurchRecRevId();
    RB_String getAccTaxPurchRecRevName();
    RB_String getAccMemoId();
    RB_String getAccMemoName();

    void getBankGlAcctIdName(RB_String& bankGlAcctId,
                             RB_String& bankGlAcctName,
                             const RB_String& bankAccountNumber) const;
    void getAndClearMessageList(RB_StringList& messageList);

    QDate getFiscalYearStart(const QDate& currentDate);
    QDate getFiscalYearEnd(const QDate& currentDate);

    bool isTaxableAccount(const RB_String& chartMasterId);

private:
    ACC_QaChartMaster();
    void initMemberValues();
    void setStandardAccounts();
    void setCompanyDefaultTaxPercentages();
    void setCompanyFiscalYearStart();

    static ACC_QaChartMaster* mActiveUtility;

    //! Account default ID
    RB_String mAccDefaultId;
    //! Account default name/description
    RB_String mAccDefaultName;

    //! Account bank/cash ID
    RB_String mAccBankId;
    //! Account bank/cash name/description
    RB_String mAccBankName;
    //! Account payable ID (debtor)
    RB_String mAccPayId;
    //! Account payable name/description
    RB_String mAccPayName;
    //! Account debtor accrued id
    RB_String mAccDebtorAccruedId;
    //! Account debtor accrued name
    RB_String mAccDebtorAccruedName;
    //! Account Receivable ID (creditor)
    RB_String mAccRecId;
    //! Account Receivable name/description
    RB_String mAccRecName;
    //! Account creditor accrued id
    RB_String mAccCreditorAccruedId;
    //! Account creditor accrued name
    RB_String mAccCreditorAccruedName;

    RB_String mAccSalesTaxLowId;
    RB_String mAccSalesTaxLowName;
    double mAccSalesTaxLowPercentage;
    RB_String mAccSalesTaxHighId;
    RB_String mAccSalesTaxHighName;
    double mAccSalesTaxHighPercentage;
    RB_String mAccSalesTaxOtherId;
    RB_String mAccSalesTaxOtherName;
    double mAccSalesTaxOtherPercentage;

    RB_String mAccPurchaseTaxLowId;
    RB_String mAccPurchaseTaxLowName;
    double mAccPurchaseTaxLowPercentage;
    RB_String mAccPurchaseTaxHighId;
    RB_String mAccPurchaseTaxHighName;
    double mAccPurchaseTaxHighPercentage;
    RB_String mAccPurchaseTaxOtherId;
    RB_String mAccPurchaseTaxOtherName;
    double mAccPurchaseTaxOtherPercentage;

    RB_String mAccSalesRevId;
    RB_String mAccSalesRevName;
    RB_String mAccPurchRevId;
    RB_String mAccPurchRevName;

    RB_String mAccTaxSalesPayRevId;
    RB_String mAccTaxSalesPayRevName;
    RB_String mAccTaxSalesRecRevId;
    RB_String mAccTaxSalesRecRevName;
    RB_String mAccTaxPurchPayRevId;
    RB_String mAccTaxPurchPayRevName;
    RB_String mAccTaxPurchRecRevId;
    RB_String mAccTaxPurchRecRevName;
    RB_String mAccMemoId;
    RB_String mAccMemoName;

    //! Account list. NOTE: parent is modelroot (=ACC_Project)
    RB_ObjectContainer* mChartMasterList;
    //! Map of chartmaster (GL accounts) id and description
    std::map<RB_String, RB_ObjectBase*> mChartMasterMap;
    //! Message list for error, warning or information messages
    RB_StringList mMessageList;

    // Fiscal year start month and day
    int mFiscalMonthStart;
    int mFiscalDayStart;
};

#endif // ACC_QACHARTMASTER_H
