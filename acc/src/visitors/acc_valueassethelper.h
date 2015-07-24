/*****************************************************************
 * $Id: acc_dbvisitor.h 1532 2012-01-23 22:44:15Z rutger $
 * Created: Apr 2, 2012 8:53:43 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_VALUEASSETHELPER_H
#define ACC_VALUEASSETHELPER_H

#include "rb_visitor.h"

class RB_ObjectContainer;


/**
 * Visitor class to create the output list from the attribute tree
 * for the ACC_ValueAsset operation
 */
class ACC_ValueAssetHelper : public RB_Visitor {

public:
    ACC_ValueAssetHelper(RB_ObjectContainer* outputList);

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);
    virtual RB_String getErrorMessage();

private:
    void createPeriodOutput(RB_ObjectBase* obj, double tmpVal);

    //! Output list with results
    RB_ObjectContainer* mOutputList;
    //! One period attribute per branch
    RB_ObjectBase* mPeriodAttr;
    //! Possible error messages
    RB_String mErrorMessage;

};

#endif // ACC_VALUEASSETHELPER_H
