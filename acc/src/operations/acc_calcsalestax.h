/*****************************************************************
 * $Id: acc_calcsalestax.h 1690 2012-07-06 16:30:32Z rutger $
 * Created: Nov 18, 2010 5:53:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CALCSALESTAX_H
#define ACC_CALCSALESTAX_H

#include "acc.h"
#include "acc_operation.h"

/**
 * This class calculates the sales tax
 * for the selected period to be shown in a report
 */
class ACC_CalcSalesTax : public ACC_Operation  {

public:
    ACC_CalcSalesTax();
    virtual ~ACC_CalcSalesTax();

    virtual bool execute(RB_ObjectBase* taxList);
    virtual void setPeriod(const QDateTime& fromPrd, const QDateTime& toPrd,
                           bool sumOnly = true);

private:
    void setQuery(RB_String& qStr, ACC2::ControlType type);
    void addObject(RB_ObjectBase* taxList, QSqlQuery& query, ACC2::ControlType type);
    void addDummyObject(RB_ObjectBase* taxList, ACC2::ControlType type);

    //! From period, year * 100 + month
    int mFromPrd;
    //! To period
    int mToPrd;
    //! True if summary report only
    bool mIsSummaryOnly;
    //! Model root ID (= project ID)
    RB_String mRootId;
};

#endif // ACC_CALCSALESTAX_H
