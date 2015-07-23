/*****************************************************************
 * $Id: db_project.cpp 2183 2014-10-02 14:33:40Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_project.h"

DB_Project::DB_Project (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


DB_Project::DB_Project(DB_Project* project) : RB_ObjectContainer(project) {
	createMembers();
	*this = *project;
}


DB_Project::~DB_Project() { 
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_Project::createMembers() {
    addMember(this, "number", "-", "DB0001", RB2::MemberChar125);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);
    addMember(this, "description", "-", "Default", RB2::MemberChar255);
    addMember(this, "company", "-", "MyCompany", RB2::MemberChar125);
    addMember(this, "location", "-", "MyLocation", RB2::MemberChar125);
}
