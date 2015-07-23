/*****************************************************************
 * $Id: rb_flagvisitor.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 12, 2011 3:49:10 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_FLAGVISITOR_H
#define RB_FLAGVISITOR_H

#include "rb.h"
#include "rb_visitor.h"


class RB_ObjectBase;

/**
 * Visitor class for setting flags of objects
 */
class DB_EXPORT RB_FlagVisitor : public RB_Visitor {
	
public:
    RB_FlagVisitor();
    virtual ~RB_FlagVisitor() {}

    virtual void visitObject(RB_ObjectBase* obj);
    virtual void leaveObject(RB_ObjectBase* obj);

    virtual void setFlag(RB2::Flags flag);
    virtual void delFlag(RB2::Flags flag);
    virtual void reset();

private:
    /** Refer rb.h and -1 is delete, 0 do not change, 1 is set flag */
    int mIsFlagIsDirty;
    int mIsFlagFromDatabase;
    int mIsFlagIsDeleted;
    int mIsFlagChildrenCreated;
};

#endif /*RB_FLAGVISITOR_H*/
