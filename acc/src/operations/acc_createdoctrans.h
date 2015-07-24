/*****************************************************************
 * $Id: acc_createdoctrans.h 2127 2014-03-20 15:16:27Z rutger $
 * Created: Nov 10, 2010 5:53:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEDOCTRANS_H
#define ACC_CREATEDOCTRANS_H

#include "acc_operation.h"

/**
 * This class prepares the GL transactions per document
 * for the selected period to be shown in a report
 */
class ACC_CreateDocTrans : public ACC_Operation  {

public:
    ACC_CreateDocTrans();
    virtual ~ACC_CreateDocTrans();

    virtual bool execute(RB_ObjectBase* docList);
    virtual void setPeriod(const QDate &fromPrd, const QDate &toPrd);

private:
    QDate mFromDate;
    QDate mToDate;
};

#endif // ACC_CREATEDOCTRANS_H
