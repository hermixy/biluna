/*****************************************************************
 * $Id: acc_postgltransaction.h 1693 2012-07-11 19:13:10Z rutger $
 * Created: Oct 7, 2010 8:53:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_POSTGLTRANSACTION_H
#define ACC_POSTGLTRANSACTION_H

#include "acc_operation.h"

/**
 * This class handles the posting or unposting of GL transactions
 * to GL Balance and Cost Center Summary.
 * The 'deleted' flag is the indicator whether the transaction needs to be
 * posted or unposted (deleted).
 */
class ACC_PostGlTransaction : public ACC_Operation  {

public:
    ACC_PostGlTransaction();
    virtual ~ACC_PostGlTransaction();

    virtual bool execute(RB_ObjectBase* glTrans);
    virtual bool createSumList(RB_ObjectContainer* glTransList);
    virtual bool postSumList();
    virtual bool recreate(int fromPrd, int toPrd);

private:
    void setDebitCredit(RB_ObjectBase* glTrans);
    bool postTransaction(RB_ObjectBase* glTrans);
    void setGlSumAmount(RB_ObjectBase* glTrans);

    bool createCostCenterSumList(RB_ObjectContainer* glTransList);
    void setCostCenterSumAmount(RB_ObjectBase* glTrans);
    bool postCostCenterSumList();
    void setCostCenterDebitCredit(RB_ObjectBase* glTrans);
    bool postCostCenter(RB_ObjectBase* glTrans);
    bool recreateCostCenterSum(int fromPrd, int toPrd);

    //! Current debit amount for GL transaction period
    double mDebitAmt;
    //! Current credit amount for GL transaction period
    double mCreditAmt;
    //! List of General Ledger Summaries
    RB_ObjectContainer* mGlSumList;

    //! Current debit amount for cost center period
    double mCostCenterDebitAmt;
    //! Current credit amount for cost center period
    double mCostCenterCreditAmt;
    //! List of cost center summaries
    RB_ObjectContainer* mCostCenterSumList;

};

#endif // ACC_POSTGLTRANSACTION_H
