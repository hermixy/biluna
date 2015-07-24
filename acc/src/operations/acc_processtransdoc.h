/*****************************************************************
 * $Id: acc_processtransdoc.h 1952 2013-07-12 17:11:46Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PROCESSTRANSDOC_H
#define ACC_PROCESSTRANSDOC_H

#include "acc_operation.h"

/**
 * Class responsible for the processing of the invoice document, including
 * updating debtortransaction, debtortaxtransaction after (part) delivery is
 * completed and confirmed, currency converted to company currency
 */
class ACC_ProcessTransDoc : public ACC_Operation  {

public:
    ACC_ProcessTransDoc();
    virtual ~ACC_ProcessTransDoc();

    virtual bool execute(RB_ObjectBase* td, RB_ObjectContainer* dctList,
                         double exchangeRate = 1.0);

private:
    bool createGlTrans();
    RB_ObjectBase* createGlHelper(const RB_String& descr, const RB_String& acctName,
                                  const RB_String& acctId, double amount);
    void setTransType(int docType);
    void setStandardAccounts();
    void updateToCompanyCurrency(double exchangeRate);

    RB_ObjectBase* mTransDoc;
    RB_ObjectContainer* mDebCredTransList;
    RB_ObjectContainer* mGlTransList;

    // Used in create new GL transaction
    RB_String mRootId;
    RB_String mDate;
    int mType;
    int mTypeNo;
    int mPeriod;
    RB_String mTransDocId;
    int mTransType;


};

#endif // ACC_PROCESSTRANSDOC_H
