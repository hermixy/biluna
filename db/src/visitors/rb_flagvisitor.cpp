/*****************************************************************
 * $Id: rb_flagvisitor.cpp 1680 2012-06-23 15:08:20Z rutger $
 * Created: Dec 12, 2011 3:48:59 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_flagvisitor.h"

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_string.h"


/**
 * Constructor
 * @param device device to be written
 */
RB_FlagVisitor::RB_FlagVisitor() {
    mIsFlagIsDirty = 0;
    mIsFlagFromDatabase = 0;
    mIsFlagIsDeleted = 0;
    mIsFlagChildrenCreated = 0;
}

/**
 * Visit object and execute Create Read Update Delete database action
 * @param obj object
 */
void RB_FlagVisitor::visitObject(RB_ObjectBase* obj) {
    if (!obj) {
        RB_DEBUG->error("RB_FlagVisitor::visitObject() object is NULL ERROR");
        return;
    }

    if (mIsFlagIsDirty == 1) {
        obj->setFlag(RB2::FlagIsDirty);
    } else if (mIsFlagIsDirty == -1) {
        obj->delFlag(RB2::FlagIsDirty);
    }

    if (mIsFlagFromDatabase == 1) {
        obj->setFlag(RB2::FlagFromDatabase);
    } else if (mIsFlagFromDatabase == -1) {
        obj->delFlag(RB2::FlagFromDatabase);
    }

    if (mIsFlagIsDeleted == 1) {
        obj->setFlag(RB2::FlagIsDeleted);
    } else if (mIsFlagIsDeleted == -1) {
        obj->delFlag(RB2::FlagIsDeleted);
    }

    if (mIsFlagChildrenCreated == 1) {
        obj->setFlag(RB2::FlagChildrenCreated);
    } else if (mIsFlagChildrenCreated == -1) {
        obj->delFlag(RB2::FlagChildrenCreated);
    }

    if (mIsFlagIsDirty == 0 && mIsFlagFromDatabase == 0
            && mIsFlagIsDeleted == 0 && mIsFlagChildrenCreated == 0) {
        obj->resetFlags();
    }
}

/**
 * Leave object, does nothing
 * @param obj object
 */
void RB_FlagVisitor::leaveObject(RB_ObjectBase* /*obj*/) {
    // nothing
}

/**
 * Set flag
 */
void RB_FlagVisitor::setFlag(RB2::Flags flag) {
    switch (flag) {
    case RB2::FlagIsDirty :
        mIsFlagIsDirty = 1;
        break;
    case RB2::FlagFromDatabase :
        mIsFlagFromDatabase = 1;
        break;
    case RB2::FlagIsDeleted :
        mIsFlagIsDeleted = 1;
        break;
    case RB2::FlagChildrenCreated :
        mIsFlagChildrenCreated = 1;
        break;
    default :
        break;
    }
}

/**
 * Delete flag
 */
void RB_FlagVisitor::delFlag(RB2::Flags flag) {
    switch (flag) {
    case RB2::FlagIsDirty :
        mIsFlagIsDirty = -1;
        break;
    case RB2::FlagFromDatabase :
        mIsFlagFromDatabase = -1;
        break;
    case RB2::FlagIsDeleted :
        mIsFlagIsDeleted = -1;
        break;
    case RB2::FlagChildrenCreated :
        mIsFlagChildrenCreated = -1;
        break;
    default :
        break;
    }
}

/**
 * Reset setting or deletion of flags,
 * will reset all flags of object
 */
void RB_FlagVisitor::reset() {
    mIsFlagIsDirty = 0;
    mIsFlagFromDatabase = 0;
    mIsFlagIsDeleted = 0;
    mIsFlagChildrenCreated = 0;
}
