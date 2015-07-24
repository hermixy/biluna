/*****************************************************************
 * $Id: peng_type.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_type.h"



PENG_Type::PENG_Type (const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


PENG_Type::PENG_Type(PENG_Type* type) : RB_ObjectContainer(type) {
	createMembers();
	*this = *type;
}

PENG_Type::~PENG_Type () {
	// deletion of children and members is done in RB_ObjectBase
	// and RB_ObjectContainer
}

/**
 * Create members.
 * This the minimum required data from the user to create a new
 * PENG_Equipment object.
 * \li typename is user type name of equipment type such as 'Column'
 * \li eqtype is a system type of the equipment for quick filtering,
 * individual datasheet, sorting for equipment list, etc.
 * \li eqsubtype is a system type for quick filtering on static or rotating types
 */
void PENG_Type::createMembers() {
    addMember(this, "typename", "-", "new type", RB2::MemberChar125);
    addMember(this, "eqtype", "-", 0, RB2::MemberInteger);
    addMember(this, "eqsubtype", "-", 0, RB2::MemberInteger);
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "field1", "-", "N.A.", RB2::MemberChar40);
    addMember(this, "unit1", "-", "N.A.", RB2::MemberChar40);
    addMember(this, "field2", "-", "N.A.", RB2::MemberChar40);
    addMember(this, "unit2", "-", "N.A.", RB2::MemberChar40);
    addMember(this, "field3", "-", "N.A.", RB2::MemberChar40);
    addMember(this, "unit3", "-", "N.A.", RB2::MemberChar40);
}
