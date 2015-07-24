/*****************************************************************
 * $Id: peng_fluid.cpp 2154 2014-07-30 19:44:11Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_fluid.h"

PENG_Fluid::PENG_Fluid (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
	
}


PENG_Fluid::PENG_Fluid(PENG_Fluid* fluid) : RB_ObjectAtomic(fluid) {
	createMembers();
	*this = *fluid;
}


PENG_Fluid::~PENG_Fluid() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void PENG_Fluid::createMembers() {
    addMember(this, "code", "-", "<NEW>", RB2::MemberChar40);
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "processutility_id", "-", 0, RB2::MemberInteger);
    addMember(this, "hazardgroup_id", "-", "0", RB2::MemberChar40);
    addMember(this, "stability_id", "-", "0", RB2::MemberChar40);
}
