/*****************************************************************
 * $Id: peng_typesection.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_typesection.h"


PENG_TypeSection::PENG_TypeSection (const RB_String& id, RB_ObjectBase* p,
                                  const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


PENG_TypeSection::PENG_TypeSection(PENG_TypeSection* typeSection) 
									: RB_ObjectContainer(typeSection) {
	createMembers();
	*this = *typeSection;
}

PENG_TypeSection::~PENG_TypeSection () {
	// deletion of children and members is done in RB_ObjectBase
	// and RB_ObjectContainer
}

/**
 * Create member definition
 * \li type data is shown to the user on datasheet and is a user understandable
 * name such as Shell Side or Tube Side
 * \li sectiontype is used as an id for the section
 * \li description is an explanation of the member
 */
void PENG_TypeSection::createMembers() {
    addMember("type", "-", "New section type", RB2::MemberChar125);	// 0
    addMember("sectiontype", "-", "", RB2::MemberChar40);	// 1
    addMember("description", "-", "", RB2::MemberChar125);		// 2
}
