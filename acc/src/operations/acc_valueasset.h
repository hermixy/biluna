/*****************************************************************
 * $Id: acc_valueasset.h 1616 2012-04-18 20:14:54Z rutger $
 * Created: Mar 31, 2012 3:53:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_VALUEASSET_H
#define ACC_VALUEASSET_H

#include "acc.h"
#include "acc_operation.h"

/**
 * This operation class calculates the asset value
 */
class ACC_ValueAsset : public ACC_Operation  {

public:
    ACC_ValueAsset();
    virtual ~ACC_ValueAsset();

    virtual bool execute(RB_ObjectBase* inputList, RB_ObjectBase* outputList);
    virtual void setValuationDate(const QDate& date);
    virtual RB_String getErrorMessage();

private:
    bool checkInputList();
    bool createAttributeTree();
    bool createOutputList();
//    void getChildren(RB_ObjectBase* obj, RB_ObjectContainer* list);
//    void getParent(RB_ObjectBase* obj, RB_ObjectBase* parent);

    //! Input data list with ACC_AssetCustAttr and ACC_AssetSupplAttr attributes
    RB_ObjectContainer* mInputList;
    //! Output result list with with objects containing date, description and money value
    RB_ObjectContainer* mOutputList;
    //! Attribute tree of objects for calculation and creation of outputList
    RB_ObjectContainer* mAttrTree;
    //! Valuation date
    QDate mDate;

    //! Error message
    RB_String mErrorMessage;

};

#endif // ACC_VALUEASSET_H
