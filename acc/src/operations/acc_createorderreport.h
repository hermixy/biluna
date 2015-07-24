/*****************************************************************
 * $Id: acc_createorderreport.h 1555 2012-02-27 18:03:50Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEORDERREPORT_H
#define ACC_CREATEORDERREPORT_H

#include "acc_operation.h"

/**
 * This class creates the list of sales- or purchase order
 */
class ACC_CreateOrderReport : public ACC_Operation  {

public:
    ACC_CreateOrderReport();
    virtual ~ACC_CreateOrderReport();

    virtual bool execute(RB_ObjectBase* trialBalanceList);
    virtual void setPeriod(const RB_String& fromPrd, const RB_String& toPrd);
    virtual void setPurchaseOrder();
    virtual void setShowAllOrders();

private:
    RB_String mFromPrd;
    RB_String mToPrd;
    bool mIsPurchaseOrder;
    bool mIsAllOrders;
};

#endif // ACC_CREATEORDERREPORT_H
