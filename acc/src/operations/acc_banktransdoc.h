/*****************************************************************
 * $Id: acc_banktransdoc.h 1999 2013-09-26 16:36:13Z rutger $
 * Created: Nov 23, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKTRANSDOC_H
#define ACC_BANKTRANSDOC_H

#include <QObject>
#include "acc_operation.h"

/**
 * Bank import transaction class, creating the documents and transactions
 * of the sales order. This operation is called from the bank import window
 * and sets the relevant (bank) documents with the transactions before
 * showing it in the bank import confirmation dialog.
 */
class ACC_BankTransDoc : public QObject, public ACC_Operation  {

    Q_OBJECT

public:
    ACC_BankTransDoc();
    virtual ~ACC_BankTransDoc();

    virtual bool execute(RB_ObjectBase* transDocList,
                         RB_ObjectBase* biList);

private:
    void setTransDocData(RB_ObjectBase* transDoc);
    void setBankTransData(RB_ObjectBase* bankTrans, RB_ObjectBase* biObj);
//    bool handleAllocation(RB_ObjectBase* bankTrans);
//    bool handleGlAccount(RB_ObjectBase* bankTrans);
    void appendTransDocDescription(const RB_String& descr);

    RB_String mBankAcctId;
    RB_String mBankAcctName;
    QDate mTransDate;
    QDate mDueDate;
    int mTransNo;
    RB_String mDocDescription;
    double mTotalAmountRec;
    double mTotalAmountPay;

};

#endif // ACC_BANKTRANSDOC_H
