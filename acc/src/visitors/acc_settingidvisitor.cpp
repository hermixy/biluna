/*****************************************************************
 * $Id: acc_settingidvisitor.cpp 1864 2013-01-10 17:45:55Z rutger $
 * Created: Jan 10, 2013 7:29 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_settingidvisitor.h"

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_debug.h"


/**
 * Constructor
 */
ACC_SettingIdVisitor::ACC_SettingIdVisitor() : RB_Visitor() {
    setResolveLevel(RB2::ResolveAll);
    mIsRelatedDone = false;
    mAction = IdNone;
}

/**
 * Destructor
 */
ACC_SettingIdVisitor::~ACC_SettingIdVisitor() {
    // nothing
}

/**
 * Visit object and execute Create Read Update database action,
 * preparation for the deletion only because children need to be deleted first
 * @param obj object
 */
void ACC_SettingIdVisitor::visitObject(RB_ObjectBase* obj) {
    if (!obj) {
        RB_DEBUG->error("ACC_SettingIdVisitor::visitObject() object is NULL ERROR");
        return;
    }

    RB_String objName = obj->getName();

    switch (mAction) {
    case IdNone :
        RB_DEBUG->error("ACC_SettingIdVisitor::visitObject() action IdNone ERROR");
        break;
    case IdRelated : {
        if (objName.compare("ACC_AccountGroup", Qt::CaseInsensitive) == 0 /* equal */) {
            RB_String newId = RB_Uuid::createUuid().toString();
            mGroupIdMap[obj->getId()] = newId;
            obj->setId(newId);
        }

        break;
    }
    case IdObject : {
        if (objName.compare("ACC_AccountGroup", Qt::CaseInsensitive) != 0 /* not equal */) {
            // Update ID only if not ACC_AccountGroup
            RB_String newId = RB_Uuid::createUuid().toString();
            obj->setId(newId);

            if (objName.compare("ACC_ChartMaster", Qt::CaseInsensitive) == 0) {
                RB_String oldId = obj->getValue("accountgroup_id").toString();
                obj->setValue("accountgroup_id", findNewGroupId(oldId));
            }
        }

        break;
    }
    default :
        RB_DEBUG->error("ACC_SettingIdVisitor::visitObject() action not set ERROR");
        break;
    }
}

/**
 * Set visitor action to set related object ID and store old- and new ID,
 * in this case only applicable for ACC_AccountGroup.
 */
void ACC_SettingIdVisitor::setRelatedObjectId() {
    mAction = IdRelated;
}

/**
 * Set visitor action to set object IDs and updated the relational ID
 * fields with the new related object IDs.
 */
void ACC_SettingIdVisitor::setObjectWithRelatedId() {
    mAction = IdObject;
}

/**
 * Find the account group ID created when inserting the ACC_ChartMaster records
 * @param oldGroupId old ACC_AccountGroup ID
 * @return ID (Uuid) new ACC_AccountGroup ID or '0' if not found
 */
RB_String ACC_SettingIdVisitor::findNewGroupId(const RB_String& oldGroupId) {
    std::map<RB_String, RB_String>::const_iterator iter;
    iter = mGroupIdMap.find(oldGroupId);

    if (iter != mGroupIdMap.end()) {
        return iter->second;
    }

    return "0";
}

