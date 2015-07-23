/*****************************************************************
 * $Id: db_systemusergroup.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: 2006 - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_systemusergroup.h"

DB_SystemUserGroup::DB_SystemUserGroup (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}


DB_SystemUserGroup::DB_SystemUserGroup(DB_SystemUserGroup* group) : RB_ObjectAtomic(group) {
	createMembers();
	*this = *group;
}


DB_SystemUserGroup::~DB_SystemUserGroup() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members of object
 * -group_idx ID of the group
 */
void DB_SystemUserGroup::createMembers() {
    addMember(this, "group_idx", "-", "0", RB2::MemberChar165);
}
