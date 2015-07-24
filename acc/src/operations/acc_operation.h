/*****************************************************************
 * $Id: acc_operation.h 1598 2012-04-03 16:11:20Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_OPERATION_H
#define ACC_OPERATION_H

#include "rb_modelfactory.h"


/**
 * Base class for operations (none-gui actions)
 * TODO: consider moving this class to DB as RB_Operation
 */
class ACC_Operation {

public:
    ACC_Operation();
    virtual ~ACC_Operation();

    virtual bool execute(RB_ObjectBase* obj = NULL);
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);

protected:
    RB_ObjectBase* mObject;

};

#endif // ACC_OPERATION_H
