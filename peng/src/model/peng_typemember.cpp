/*****************************************************************
 * $Id: peng_typemember.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_typemember.h"

PENG_TypeMember::PENG_TypeMember (const RB_String& id, RB_ObjectBase* p,
                                const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


PENG_TypeMember::PENG_TypeMember(PENG_TypeMember* member) : RB_ObjectContainer(member) {
	createMembers();
	*this = *member;
}

PENG_TypeMember::~PENG_TypeMember () {
	// deletion of children and members is done in RB_ObjectBase
	// and RB_ObjectContainer
}

/**
 * Create members for equipment type:
 * \li mname is the variable name as used in the application and database creation
 * \li unit is the unit of measurement, for text the unit is a 'dash'
 * \li value type is an integer corresponding with the RB2::MemberType, for example
 * 6 is a double (RB2::MemberDouble). Check RB2::MemberType for possible changes
 */
void PENG_TypeMember::createMembers() {
    addMember(this, "mname", "-", "memberName", RB2::MemberChar40);	// 0
    addMember(this, "unit", "-", "-", RB2::MemberChar40);				// 1
    addMember(this, "valuetype", "-", 0, RB2::MemberInteger);			// 2
    // addMember(this, "pValue", "-", "");							// 3
}
