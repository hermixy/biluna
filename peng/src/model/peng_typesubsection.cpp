/*****************************************************************
 * $Id: peng_typesubsection.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_typesubsection.h"

PENG_TypeSubsection::PENG_TypeSubsection (const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


PENG_TypeSubsection::PENG_TypeSubsection(PENG_TypeSubsection* typeSubsection) 
									: RB_ObjectContainer(typeSubsection) {
	createMembers();
	*this = *typeSubsection;
}

PENG_TypeSubsection::~PENG_TypeSubsection () {
	// deletion of children and members is done in RB_ObjectBase
	// and RB_ObjectContainer
}

/**
 * Create member definition
 * \li type data is shown to the user on datasheet and is a user understandable
 * name such as Shell Side Inlet or Shell Side Outlet  
 * \li subsectiontype is used as an id for the section
 * \li description is an explanation of the member
 */
void PENG_TypeSubsection::createMembers() {
    addMember(this, "type", "-", "New subsection type", RB2::MemberChar125);	// 0
    addMember(this, "subsectiontype", "-", "", RB2::MemberChar40);		// 1
    addMember(this, "description", "-", "", RB2::MemberChar125);			// 2
}
