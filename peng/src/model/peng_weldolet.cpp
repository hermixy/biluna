/*****************************************************************
 * $Id: peng_weldolet.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 15, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_weldolet.h"

PENG_Weldolet::PENG_Weldolet (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


PENG_Weldolet::PENG_Weldolet(PENG_Weldolet* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


PENG_Weldolet::~PENG_Weldolet() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void PENG_Weldolet::createMembers() {
    addMember(this, "number", "-", "<number>", RB2::MemberChar20);   // document number
    addMember(this, "description", "-", "<description>", RB2::MemberChar125);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);         // issue

    addMember(this, "D0", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "D2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "D21", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "D3", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "gap", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "gap1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "H3", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "alpha1", "degree", 0.0, RB2::MemberDouble);
    addMember(this, "alpha2", "degree", 0.0, RB2::MemberDouble);
    addMember(this, "alpha3", "degree", 0.0, RB2::MemberDouble);

    addMember(this, "steps", "-", 0, RB2::MemberInteger);         // iteration steps
    addMember(this, "textOutput", "-", 1, RB2::MemberInteger);    // yes/no boolean
    addMember(this, "zoomFactor", "-", 1.0, RB2::MemberDouble);
}
