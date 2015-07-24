/*****************************************************************
 * $Id: acc_valuateasset.h 1596 2012-03-31 15:02:45Z rutger $
 * Created: Mar 31, 2012 3:53:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_VALUATEASSET_H
#define ACC_VALUATEASSET_H

#include "acc.h"
#include "acc_operation.h"

/**
 * This operation class calculates the asset value
 */
class ACC_ValuateAsset : public ACC_Operation  {

public:
    ACC_ValuateAsset();
    virtual ~ACC_ValuateAsset();

    virtual bool execute(RB_ObjectBase* inputList, RB_ObjectBase* outputList);
    virtual void setValuationDate(const QDate& date);

};

#endif // ACC_VALUATEASSET_H
