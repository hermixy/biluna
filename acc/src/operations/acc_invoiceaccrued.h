/*****************************************************************
 * $Id: acc_invoiceaccrued.h 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 11, 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_INVOICEACCRUED_H
#define ACC_INVOICEACCRUED_H

#include "acc_operation.h"


/**
 * Operation class for sales and purchase invoices accrued,
 * invoices open at year closing
 */
class ACC_InvoiceAccrued : public ACC_Operation  {

public:
    ACC_InvoiceAccrued();
    virtual ~ACC_InvoiceAccrued();

    virtual bool execute(RB_ObjectContainer* invoiceAccruedList = NULL);
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);

    virtual void setDebtorCreditor(bool isDebtor) {
        mIsDebtor = isDebtor;
    }

    virtual void setEndDate(const QDate& endDate) {
        mEndDate = endDate;
    }

protected:
    bool mIsDebtor;
    QDate mEndDate;
};

#endif // ACC_INVOICEACCRUED_H
