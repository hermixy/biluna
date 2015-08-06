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
    addMember("number", "-", "<number>", RB2::MemberChar20);   // document number
    addMember("description", "-", "<description>", RB2::MemberChar125);
    addMember("revision", "-", 0, RB2::MemberInteger);         // issue

    addMember("D0", "mm", 0.0, RB2::MemberDouble);
    addMember("D2", "mm", 0.0, RB2::MemberDouble);
    addMember("D21", "mm", 0.0, RB2::MemberDouble);
    addMember("D3", "mm", 0.0, RB2::MemberDouble);
    addMember("gap", "mm", 0.0, RB2::MemberDouble);
    addMember("gap1", "mm", 0.0, RB2::MemberDouble);
    addMember("H3", "mm", 0.0, RB2::MemberDouble);
    addMember("alpha1", "degree", 0.0, RB2::MemberDouble);
    addMember("alpha2", "degree", 0.0, RB2::MemberDouble);
    addMember("alpha3", "degree", 0.0, RB2::MemberDouble);

    addMember("steps", "-", 0, RB2::MemberInteger);         // iteration steps
    addMember("textOutput", "-", 1, RB2::MemberInteger);    // yes/no boolean
    addMember("zoomFactor", "-", 1.0, RB2::MemberDouble);
}
