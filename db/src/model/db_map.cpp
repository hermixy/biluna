/*****************************************************************
 * $Id: db_map.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_map.h"


DB_Map::DB_Map(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

DB_Map::DB_Map(DB_Map* map) : RB_ObjectContainer(map) {
	createMembers();
	*this = *map;
}
	
DB_Map::~DB_Map() {
	// clean up of members and children is done in RB_ObjectBase
	// and ObjectContainer
}

/**
 * Create members:
 * @param number identification number or description, database-table-fileId
 * @param mdatabase database name
 * @param mtable name of table where data is to be inserted
 * @param createid true/false: is an ID to be created for each row
 * @param parent_id id of parent (expect the field name to be id
 * @param importfile path and file name
 * @param fileheading true/false: does CSV has a header row?
 */
void DB_Map::createMembers() {
    addMember("number", "-", "", RB2::MemberChar125);
    addMember("mdatabase", "-", "", RB2::MemberChar125);
    addMember("mtable", "-", "", RB2::MemberChar125);
    addMember("createid", "-", 0, RB2::MemberInteger);
    addMember("parent_id", "-", "0", RB2::MemberChar40);
    addMember("importfile", "-", "", RB2::MemberChar125);
    addMember("fileheading", "-", 0, RB2::MemberInteger);
}
