/*****************************************************************
 * $Id: db_permissionproject.cpp 2144 2014-05-01 18:22:24Z rutger $
 * Created: 2007 - rutger
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna AUTH project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissionproject.h"

DB_PermissionProject::DB_PermissionProject (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


DB_PermissionProject::DB_PermissionProject(DB_PermissionProject* projectRoot)
                    : RB_ObjectContainer(projectRoot) {
	createMembers();
	*this = *projectRoot;
}


DB_PermissionProject::~DB_PermissionProject() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members of object
 * -persproject_idx, project of a perspective
 * -description, description of the project
 * -customer, customer or owner name
 * -mstatus_id, status or Type of access to data live (view+edit)
 *   locked (view only) hidden (admin only)
 * -start, start date from which this project is valid for viewing and editing,
 *   viewing overrules editing set by authorization
 * -end, end date after which this project is not visible or editable by anybody
 */
void DB_PermissionProject::createMembers() {
    addMember("persproject_idx", "-", "0", RB2::MemberChar165);
    addMember("description", "-", "", RB2::MemberChar125);
    addMember("customer", "-", "", RB2::MemberChar125);
    addMember("mstatus_id", "-", 0, RB2::MemberInteger);
    addMember("start", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("end", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
}
