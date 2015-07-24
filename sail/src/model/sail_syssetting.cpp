/*****************************************************************
 * $Id: sail_syssetting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_syssetting.h"

SAIL_SysSetting::SAIL_SysSetting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SAIL_SysSetting::SAIL_SysSetting(SAIL_SysSetting* obj)
        : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SAIL_SysSetting::~SAIL_SysSetting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/*!
  Create members:
  \li syssetingname system setting name (placeholder only)
 */
void SAIL_SysSetting::createMembers() {
    addMember(this, "syssettingname", "-", "Placeholder only", RB2::MemberChar40);
}
