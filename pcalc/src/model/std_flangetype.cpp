/*****************************************************************
 * $Id: std_flangetype.cpp 0001 2016-07-13T15:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flangetype.h"

STD_FlangeType::STD_FlangeType(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_FlangeType::STD_FlangeType(STD_FlangeType* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_FlangeType::~STD_FlangeType() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li type flange type number or name
 * \li description description of flange type
 */
void STD_FlangeType::createMembers() {
    addMember("type", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar125);
}
