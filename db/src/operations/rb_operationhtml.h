/*****************************************************************
 * $Id: rb_operationhtml.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 5, 2013 10:14:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_OPERATIONHTML_H
#define RB_OPERATIONHTML_H

#include "db_global.h"
#include "rb_modelfactory.h"


/**
 * Base class for HTML creation operations
 */
class DB_EXPORT RB_OperationHtml {

public:
    RB_OperationHtml();
    virtual ~RB_OperationHtml();

    virtual bool execute(RB_ObjectBase* obj = NULL);
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);
    virtual RB_String getOnePixelTransparentImage() const;

protected:
    RB_ObjectBase* mObject;

};

#endif // RB_OPERATIONHTML_H
