/*****************************************************************
 * $Id: peng_cone.cpp 2163 2014-09-04 08:03:35Z rutger $
 * Created: Aug 10, 2011 8:04:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_cone.h"

PENG_Cone::PENG_Cone (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


PENG_Cone::PENG_Cone(PENG_Cone* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


PENG_Cone::~PENG_Cone() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void PENG_Cone::createMembers() {
    addMember(this, "number", "-", "<NEW>", RB2::MemberChar20);
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);
    addMember(this, "D0", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "D1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "t0", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "t1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "H0", "mm", 0.0, RB2::MemberDouble);
}
