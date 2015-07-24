/*****************************************************************
 * $Id: acc_creategltrialbalance.h 1320 2010-11-10 15:55:03Z rutger $
 * Created: Oct 7, 2010 8:53:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEGLTRIALBALANCE_H
#define ACC_CREATEGLTRIALBALANCE_H

#include "acc_operation.h"

/**
 * This class creates trial balances for any date range that recalculates
 * the p & l balances and shows the balance sheets as at the end of
 * the period selected
 */
class ACC_CreateGlTrialBalance : public ACC_Operation  {

public:
    ACC_CreateGlTrialBalance();
    virtual ~ACC_CreateGlTrialBalance();

    virtual bool execute(RB_ObjectBase* trialBalanceList);
    virtual void setPeriod(int fromPrd, int toPrd);

private:
    int mFromPrd;
    int mToPrd;
};

#endif // ACC_CREATEGLTRIALBALANCE_H
