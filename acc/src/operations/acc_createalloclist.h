/*****************************************************************
 * $Id: acc_createalloclist.h 2127 2014-03-20 15:16:27Z rutger $
 * Created: June 29, 2011 19:29:42 PM - rutger
 *
 * Copyright (C) 2011 Biluna. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CREATEALLOCLIST_H
#define ACC_CREATEALLOCLIST_H

#include "acc.h"
#include "acc_operation.h"


/**
 * This class prepares the amount allocated report. Allocation are made
 * to documents (sales invoices or purchase invoices) from items
 * of Bank/Cash documents or items of Journal documents. This report
 * is used to check which allocation are made to a document
 * and solve possible faulty entries.
 */
class ACC_CreateAllocList : public ACC_Operation {

public:
    ACC_CreateAllocList();
    virtual ~ACC_CreateAllocList();

    virtual bool execute(RB_ObjectBase* allocList);
    virtual void setPeriod(const QDate& startDate,
                           const QDate& endDate);

private:
    void setQuery(RB_String& qStr, ACC2::ControlType docToType,
                  ACC2::ControlType docFromType);
    void addObjectsToList(QSqlQuery& q, RB_ObjectBase* allocList);

    QDate mStartDate;
    QDate mEndDate;
};

#endif // ACC_CREATEALLOCLIST_H
