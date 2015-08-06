/*****************************************************************
 * $Id: db_mapping.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_mapping.h"


DB_Mapping::DB_Mapping(const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
						: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

DB_Mapping::DB_Mapping(DB_Mapping* map) : RB_ObjectAtomic(map) {
	createMembers();
	*this = *map;
}
	
DB_Mapping::~DB_Mapping() {
	// clean up of members and children is done in RB_ObjectBase
	// and ObjectContainer
}

/**
 * Create members:
 * @param columnnumber column number
 * @param tofield field to which data should be mapped
 */
void DB_Mapping::createMembers() {
    addMember("columnnumber", "-", 0, RB2::MemberInteger);
    addMember("tofield", "-", "", RB2::MemberChar125);
}
