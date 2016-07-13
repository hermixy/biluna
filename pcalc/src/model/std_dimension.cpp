/*****************************************************************
 * $Id: std_dimension.cpp 0001 2016-07-07T17:02:15 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_dimension.h"

STD_Dimension::STD_Dimension(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_Dimension::STD_Dimension(STD_Dimension* obj) : 
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_Dimension::~STD_Dimension() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li code code, number or abbreviation of dimension standard
 * \li edition edition, issue or revision of the standard
 * \li title title of dimension standard
 * \li description description or subtitle
 * \li comptype_id component type id, for filtering
 */
void STD_Dimension::createMembers() {
    addMember("code", "-", "<NEW>", RB2::MemberChar125);
    addMember("edition", "-", "", RB2::MemberChar125);
    addMember("title", "-", "", RB2::MemberChar255);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("comptype_id", "-", 0, RB2::MemberInteger);
}
