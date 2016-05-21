/*****************************************************************
 * $Id: std_rmmin.cpp 0001 2016-05-21T10:17:36 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_rmmin.h"

STD_RmMin::STD_RmMin(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_RmMin::STD_RmMin(STD_RmMin* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_RmMin::~STD_RmMin() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li rmmin stress [MPa]
 */
void STD_RmMin::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("rmmin", "MPa", 0.0, RB2::MemberDouble);
}
