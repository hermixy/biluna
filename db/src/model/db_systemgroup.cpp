/*****************************************************************
 * $Id: db_systemgroup.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: 2014-04-24 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_systemgroup.h"

DB_SystemGroup::DB_SystemGroup (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


DB_SystemGroup::DB_SystemGroup(DB_SystemGroup* projectRoot)
                    : RB_ObjectContainer(projectRoot) {
	createMembers();
	*this = *projectRoot;
}


DB_SystemGroup::~DB_SystemGroup() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members of object
 * \li code, short name or code for this group
 * \li description, description of the group
 * \li permission_id, None, [R] Read, [RU] Read Update,
 *  [CRUD] Create Read Update Delete, [CRUDX] Create Read Update Delete Execute
 */
void DB_SystemGroup::createMembers() {
    addMember("code", "-", "<NEW>", RB2::MemberChar40);
    addMember("description", "-", "", RB2::MemberChar125);
    addMember("permission_id", "-", 0, RB2::MemberInteger);
}
