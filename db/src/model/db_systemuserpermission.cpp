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
 * \li userstart
 * \li userend
 * \li groupstatus_id
 * \li grouppermission_id
 * \li groupstart
 * \li groupend
 * \li persproject_idx {Uuid}tablename - project number
 * \li persprojectstatus_id
 * \li persprojectstart
 * \li persprojectend
 */
void DB_SystemUserPermission::createMembers() {
    addMember("userstart", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("userend", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("groupstatus_id", "-", 0, RB2::MemberInteger);
    addMember("grouppermission_id", "-", 0, RB2::MemberInteger);
    addMember("groupstart", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("groupend", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("persproject_idx", "-", "", RB2::MemberChar165);
    addMember("persprojectstatus_id", "-", 0, RB2::MemberInteger);
    addMember("persprojectstart", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("persprojectend", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
}
