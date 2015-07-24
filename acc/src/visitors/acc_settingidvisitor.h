/*****************************************************************
 * $Id: acc_settingidvisitor.h 1864 2013-01-10 17:45:55Z rutger $
 * Created: Jan 10, 2013 7:29 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SETTINGIDVISITOR_H
#define ACC_SETTINGIDVISITOR_H

#include <map>
#include "rb_visitor.h"


/**
 * Visitor class that updates the ID's before the objects are inserted
 * in the database. The related accountgroup_id in acc_chartmaster
 * is also updated with the new acc_accountgroup. Therefore this visitor
 * has to be used twice, first with setRelatedObjectId()
 * second with setObjectWithRelatedId()
 */
class ACC_SettingIdVisitor : public RB_Visitor {

public:
    ACC_SettingIdVisitor();
    virtual ~ACC_SettingIdVisitor();

    virtual void visitObject(RB_ObjectBase* obj);

    virtual void setRelatedObjectId();
    virtual void setObjectWithRelatedId();

private:
    bool isRelatedDone();
    RB_String findNewGroupId(const RB_String& oldGroupId);

    bool mIsRelatedDone;
    std::map<RB_String, RB_String> mGroupIdMap;

    enum IdAction {
        IdNone,
        IdRelated,  // Set the related object ID's first
        IdObject    // Set the objects and the ID's of the related fields (_id)
    };

    IdAction mAction;
};

#endif // ACC_SETTINGIDVISITOR_H
