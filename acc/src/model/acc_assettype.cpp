/*****************************************************************
 * $Id: acc_assettype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assettype.h"


ACC_AssetType::ACC_AssetType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_AssetType::ACC_AssetType(ACC_AssetType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_AssetType::~ACC_AssetType() {
    // nothing
}

/**
 * Create members:
 * - mname type name of the assets
 * - description description of the type
 */
void ACC_AssetType::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
}
