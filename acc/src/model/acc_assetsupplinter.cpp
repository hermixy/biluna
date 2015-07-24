/*****************************************************************
 * $Id: acc_assetsupplinter.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assetsupplinter.h"


ACC_AssetSupplInter::ACC_AssetSupplInter (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_AssetSupplInter::ACC_AssetSupplInter(ACC_AssetSupplInter* obj)
                    : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_AssetSupplInter::~ACC_AssetSupplInter() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - suppl_id ID of supplier interface
 */
void ACC_AssetSupplInter::createMembers() {
    addMember(this, "suppl_id", "-", "0", RB2::MemberChar40);
}
