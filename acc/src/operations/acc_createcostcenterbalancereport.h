/*****************************************************************
 * $Id: acc_createcostcenterbalancereport.h 1935 2013-05-23 16:47:33Z rutger $
 * Created: Sept 29, 2012 12:13:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATECOSTCENTERBALANCEREPORT_H
#define ACC_CREATECOSTCENTERBALANCEREPORT_H

#include <QDate>
#include "acc_operation.h"

/**
 * This class creates cost center balance report data for any date range
 */
class ACC_CreateCostCenterBalanceReport : public ACC_Operation  {

public:
    ACC_CreateCostCenterBalanceReport();
    virtual ~ACC_CreateCostCenterBalanceReport();

    virtual bool execute(RB_ObjectBase* costCenterBalanceList);
    virtual void setPeriod(int fromPrd, int toPrd);

private:
    void setCustomerCostGroupSql(RB_String& qStr);
    void setCustomerCostCenterSql(RB_String& qStr);
    void setSupplierCostGroupSql(RB_String& qStr);
    void setSupplierCostCenterSql(RB_String& qStr);
    void setCostCenterBalanceList(QSqlQuery& q,
                                  RB_ObjectBase* costCenterBalanceList);

    RB_String mProjectId;
    int mFromPrd;
    int mToPrd;
    QDate mFromDate;
    QDate mToDate;
};

#endif // ACC_CREATECOSTCENTERBALANCEREPORT_H
