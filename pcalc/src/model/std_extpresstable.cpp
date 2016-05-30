/*****************************************************************
 * $Id: std_extpresstable.cpp 0001 2016-05-21T09:42:52 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_extpresstable.h"

STD_ExtPressTable::STD_ExtPressTable(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_ExtPressTable::STD_ExtPressTable(STD_ExtPressTable* obj) : 
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_ExtPressTable::~STD_ExtPressTable() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname name of external pressure table
 */
void STD_ExtPressTable::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar125);

    // temporary for importing of external data only
    addMember("tmp_id", "-", "0", RB2::MemberChar125);
    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
