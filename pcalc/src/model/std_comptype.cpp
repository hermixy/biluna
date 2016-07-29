/*****************************************************************
 * $Id: std_comptype.cpp 0001 2016-07-13T15:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_comptype.h"

STD_CompType::STD_CompType(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_CompType::STD_CompType(STD_CompType* obj) :
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_CompType::~STD_CompType() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li type flange type number or name
 * \li description description of flange type
 */
void STD_CompType::createMembers() {
    addMember("type", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar125);
}
