/*****************************************************************
 * $Id: acc_createbankpaymentlist.h 1734 2012-08-16 13:12:19Z rutger $
 * Created: Oct 14, 2015 21:17:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEBANKPAYMENTLIST_H
#define ACC_CREATEBANKPAYMENTLIST_H

#include "rb_modelfactory.h"


/**
 * Operation class for software license report
 */
class ACC_CreateBankPaymentList {

public:
    ACC_CreateBankPaymentList();
    virtual ~ACC_CreateBankPaymentList();

    virtual bool execute(RB_ObjectContainer* licenseList = NULL);
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);

    virtual void setLimits(const RB_String& field,
                           const RB_String& fieldFrom,
                           const RB_String& fieldTo) {
        mField = field;
        mFieldFrom = fieldFrom;
        mFieldTo = fieldTo;
    }

protected:
    RB_ObjectBase* mObject;
    RB_String mField;
    RB_String mFieldFrom;
    RB_String mFieldTo;
};

#endif // ACC_CREATEBANKPAYMENTLIST_H
