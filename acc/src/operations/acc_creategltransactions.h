/*****************************************************************
 * $Id: acc_creategltransactions.h 2127 2014-03-20 15:16:27Z rutger $
 * Created: Nov 8, 2010 5:53:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEGLTRANSACTIONS_H
#define ACC_CREATEGLTRANSACTIONS_H

#include "acc_operation.h"

/**
 * This class prepares the GL transactions per GL account
 * for the selected period and accounts to be shown in a report
 */
class ACC_CreateGlTransactions : public ACC_Operation  {

public:
    ACC_CreateGlTransactions();
    virtual ~ACC_CreateGlTransactions();

    virtual bool execute(RB_ObjectBase* glTransList);
    virtual void setPeriod(const QDate& fromPrd, const QDate& toPrd);
    virtual void setAccountRange(const RB_String& fromAcct, const RB_String& toAcct);

private:
    QDate mFromDate;
    QDate mToDate;
    RB_String mFromAcct;
    RB_String mToAcct;
};

#endif // ACC_CREATEGLTRANSACTIONS_H
