/*****************************************************************
 * $Id: rb_operation.h 0001 2015-12-28 09:13:00Z rutger $
 * Created: Dec 28, 2015 10:14:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_OPERATION_H
#define RB_OPERATION_H

#include "db_global.h"
#include "rb_debug.h"
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"

/**
 * Abstract class for operations
 */
class DB_EXPORT RB_Operation {

public:
    RB_Operation() {}
    virtual ~RB_Operation() {}

    virtual bool execute(RB_ObjectBase* obj = NULL) = 0;
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output) = 0;
protected:
    RB_ObjectBase* mObject;
    RB_ObjectBase* mInObject;
    RB_ObjectBase* mOutObject;

};

#endif // RB_OPERATION_H

