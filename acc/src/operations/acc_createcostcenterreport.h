/*****************************************************************
 * $Id: acc_createcostcenterreport.h 1696 2012-07-12 15:15:28Z rutger $
 * Created: Jul 9, 2012 6:48:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATECOSTCENTERREPORT_H
#define ACC_CREATECOSTCENTERREPORT_H

#include "acc_operation.h"

/**
 * This class creates cost center report for any date range
 */
class ACC_CreateCostCenterReport : public ACC_Operation  {

public:
    ACC_CreateCostCenterReport();
    virtual ~ACC_CreateCostCenterReport();

    virtual bool execute(RB_ObjectBase* costCenterSumList);
    virtual void setPeriod(int fromPrd, int toPrd);

private:
    int mFromPrd;
    int mToPrd;
};

#endif // ACC_CREATECOSTCENTERREPORT_H
