/*****************************************************************
 * $Id: db_systemuserpermission.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: 2006 - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_systemuserpermission.h"

DB_SystemUserPermission::DB_SystemUserPermission (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}


DB_SystemUserPermission::DB_SystemUserPermission(DB_SystemUserPermission* user)
                    : RB_ObjectAtomic(user) {
	createMembers();
	*this = *user;
}


DB_SystemUserPermission::~DB_SystemUserPermission() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create object members
 * \li username
 * \li firstname
 * \li lastname
 * \li userstart
 * \li userend
 * \li crudx_id
 * \li tokenlist
 * \li persproject_idx {Uuid}tablename - project number
 * \li persprojectstatus_id
 * \li persprojectstart
 * \li persprojectend
 */
void DB_SystemUserPermission::createMembers() {
    addMember("username", "-", "", RB2::MemberChar125);
    addMember("firstname", "-", "", RB2::MemberChar125);
    addMember("lastname", "-", "", RB2::MemberChar125);
    addMember("userstart", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("userend", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("crudx_id", "-", 0, RB2::MemberInteger);
    addMember("tokenlist", "-", 0, RB2::MemberInteger);
    addMember("persproject_idx", "-", "", RB2::MemberChar165);
    addMember("persprojectstatus_id", "-", 0, RB2::MemberInteger);
    addMember("persprojectstart", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("persprojectend", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
}
