/*****************************************************************
 * $Id: peng_fluidstability.cpp 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2014-07-29 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_fluidstability.h"


/**
 * Constructor
 */
PENG_FluidStability::PENG_FluidStability (const RB_String& id, RB_ObjectBase* p,
                                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	
    createMembers();
}


PENG_FluidStability::PENG_FluidStability(PENG_FluidStability* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


PENG_FluidStability::~PENG_FluidStability() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * name - hazard group name
 * description - hazard group description
 */
void PENG_FluidStability::createMembers() {
    addMember("code", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
}

