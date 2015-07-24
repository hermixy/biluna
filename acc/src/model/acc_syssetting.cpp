/*****************************************************************
 * $Id: acc_syssetting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Dec 13, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_syssetting.h"

ACC_SysSetting::ACC_SysSetting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SysSetting::ACC_SysSetting(ACC_SysSetting* obj)
        : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SysSetting::~ACC_SysSetting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - syssettingname system setting name
 */
void ACC_SysSetting::createMembers() {
    addMember(this, "syssettingname", "-", "Placeholder only", RB2::MemberChar40);      // 3
}
