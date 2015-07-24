/*****************************************************************
 * $Id: acc_createcostcenter3report.h 1696 2012-07-12 15:15:28Z rutger $
 * Created: Jul 12, 2012 2:48:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATECOSTCENTER3REPORT_H
#define ACC_CREATECOSTCENTER3REPORT_H

#include "acc_operation.h"

/**
 * This class creates cost center report for three (3) periods,
 * 3 moths or 3 years
 */
class ACC_CreateCostCenter3Report : public ACC_Operation  {

public:
    ACC_CreateCostCenter3Report();
    virtual ~ACC_CreateCostCenter3Report();

    virtual bool execute(RB_ObjectBase* costCenterSumList);
    virtual void setPeriod(int prd1, int prd2, int prd3);

private:
    void execPeriod(RB_ObjectBase* costCenterSumList, int period);
    void setPeriod1(RB_ObjectBase* costCenterSumList, QSqlQuery& query);
    void setPeriod2(RB_ObjectBase* costCenterSumList, QSqlQuery& query);
    void setPeriod3(RB_ObjectBase* costCenterSumList, QSqlQuery& query);

    int mPrd1;
    int mPrd2;
    int mPrd3;

    int mFromPrd;
    int mToPrd;
    bool mIsPeriodYear;
};

#endif // ACC_CREATECOSTCENTER3REPORT_H
