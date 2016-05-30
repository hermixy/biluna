/*****************************************************************
 * $Id: std_thermexptable.cpp 0001 2016-05-21T09:36:30 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_thermexptable.h"

STD_ThermExpTable::STD_ThermExpTable(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_ThermExpTable::STD_ThermExpTable(STD_ThermExpTable* obj) : 
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_ThermExpTable::~STD_ThermExpTable() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname name of thermal expansion table
 */
void STD_ThermExpTable::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar125);

    // temporary for importing of external data only
    addMember("tmp_id", "-", "0", RB2::MemberChar125);
    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
