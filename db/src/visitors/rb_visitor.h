/*****************************************************************
 * $Id: rb_visitor.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 21, 2011 7:29 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_VISITOR_H
#define RB_VISITOR_H

#include "db_global.h"
#include "rb.h"


class RB_ObjectBase;

/**
 * Abstract base class for visitor pattern. Is used for example
 * for writing XML to a file.
 */
class DB_EXPORT RB_Visitor {

public:
    virtual ~RB_Visitor();

    virtual void visitObject(RB_ObjectBase* obj) = 0;
    virtual void leaveObject(RB_ObjectBase* /*obj*/) { }

    virtual RB2::ResolveLevel getResolveLevel();
    virtual void setResolveLevel(RB2::ResolveLevel level);
    virtual RB2::ResolveLevel nextResolveLevel();
    virtual RB2::ResolveLevel previousResolveLevel();

protected:
    RB_Visitor();

private:
    RB2::ResolveLevel mResolveLevel;
};

#endif // RB_VISITOR_H
