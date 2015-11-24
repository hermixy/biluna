/*****************************************************************
 * $Id: db_permissionperspectiveproject.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: 2015-01-04
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissionperspectiveproject.h"

DB_PermissionPerspectiveProject::DB_PermissionPerspectiveProject (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


DB_PermissionPerspectiveProject::DB_PermissionPerspectiveProject(DB_PermissionPerspectiveProject* project) : RB_ObjectContainer(project) {
	createMembers();
	*this = *project;
}


DB_PermissionPerspectiveProject::~DB_PermissionPerspectiveProject() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_PermissionPerspectiveProject::createMembers() {
    addMember("perspective", "-", "", RB2::MemberChar10);
    addMember("number", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("company", "-", "", RB2::MemberChar125);
    addMember("location", "-", "", RB2::MemberChar125);
}
