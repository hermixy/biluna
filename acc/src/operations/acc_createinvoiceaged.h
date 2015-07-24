/*****************************************************************
 * $Id: acc_createinvoiceaged.h 1847 2012-12-23 14:40:07Z rutger $
 * Created: Dec 8, 2010 12:29:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEINVOICEAGED_H
#define ACC_CREATEINVOICEAGED_H

#include "acc_operation.h"

/**
 * This class prepares the invoices aged report, customer and supplier,
 * for the selected period and accounts to be shown in a report
 */
class ACC_CreateInvoiceAged : public ACC_Operation  {

public:
    ACC_CreateInvoiceAged();
    virtual ~ACC_CreateInvoiceAged();

    virtual bool execute(RB_ObjectBase* invoiceList);
    virtual void setOverDue(int overDueDays1, int overDueDays2);
    virtual void setCustSuppRange(const RB_String& fromCustSupp, const RB_String& toCustSupp);

    virtual void setDocType(int docType) { mDocType = docType; }

private:
    void setQuery(RB_String& qStr);

    int mOverDueDays1;
    int mOverDueDays2;
    RB_String mFromCustSupp;
    RB_String mToCustSupp;

    RB_String mToDaysOpen;
    RB_String mNowDateTime;
    RB_String mCustSuppName;
    RB_String mCustSupp;
    RB_String mCustSuppIdName;
    RB_String mTotalAmountRecPay;
    RB_String mTotalAmountRecPayPlMin;
    int mDocType;
    RB_String mDocTypeSql;
};

#endif // ACC_CREATEINVOICEAGED_H
