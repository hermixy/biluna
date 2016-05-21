/*****************************************************************
 * $Id: std_creeptable.cpp 0001 2016-05-21T09:30:40 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_creeptable.h"

STD_CreepTable::STD_CreepTable(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_CreepTable::STD_CreepTable(STD_CreepTable* obj) : 
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_CreepTable::~STD_CreepTable() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname Name of creep table
 */
void STD_CreepTable::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar125);
}
