/*****************************************************************
 * $Id: peng_paint.cpp 2163 2014-09-04 08:03:35Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Plant Engineering (PENG) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_paint.h"



PENG_Paint::PENG_Paint(const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}


PENG_Paint::PENG_Paint(PENG_Paint* paint) : RB_ObjectAtomic(paint) {
	createMembers();
	*this = *paint;
}
	
	
PENG_Paint::~PENG_Paint() {
    // clean up of members and children is done in RB_ObjectBase
	// and ObjectContainer
}


void PENG_Paint::createMembers() {
    addMember("number", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar125);
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("applicable_id", "-", 0, RB2::MemberInteger);
    addMember("temperaturelimithigh", "DegC", 0.0, RB2::MemberDouble);
    addMember("temperaturelimitlow", "DegC", 0.0, RB2::MemberDouble);
    addMember("comment", "-", "", RB2::MemberChar255);
    addMember("surfacepreparation", "-", "", RB2::MemberChar255);
    addMember("primercoat", "-", "", RB2::MemberChar255);
    addMember("finishcoat1", "-", "", RB2::MemberChar255);
    addMember("finishcoat2", "-", "", RB2::MemberChar255);
    addMember("finishcoat3", "-", "", RB2::MemberChar255);
}
