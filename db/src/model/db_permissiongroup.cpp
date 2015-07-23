/*****************************************************************
 * $Id: db_permissiongroup.cpp 2222 2015-02-26 19:38:49Z rutger $
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna AUTH project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissiongroup.h"

DB_PermissionGroup::DB_PermissionGroup(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory *f)
                    : RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}


DB_PermissionGroup::DB_PermissionGroup(DB_PermissionGroup* group)
                    : RB_ObjectAtomic(group) {
	createMembers();
	*this = *group;
}


DB_PermissionGroup::~DB_PermissionGroup() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members of Object
 * -group_idx, id and name or number of the group
 */
void DB_PermissionGroup::createMembers() {
    addMember(this, "group_idx", "-", "0", RB2::MemberChar165);
}
