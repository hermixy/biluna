/*****************************************************************
 * $Id: peng_line.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2014-06-16 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_hazardgroup.h"


/**
 * Constructor
 */
PENG_HazardGroup::PENG_HazardGroup (const RB_String& id, RB_ObjectBase* p,
                                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	
    createMembers();
}


PENG_HazardGroup::PENG_HazardGroup(PENG_HazardGroup* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


PENG_HazardGroup::~PENG_HazardGroup() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * name - hazard group name
 * description - hazard group description
 */
void PENG_HazardGroup::createMembers() {
    addMember(this, "code", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "description", "-", "", RB2::MemberChar255);
}

