/*****************************************************************
 * $Id: db_systemuser.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: 2006 - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_systemuser.h"

DB_SystemUser::DB_SystemUser (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	createMembers();
}


DB_SystemUser::DB_SystemUser(DB_SystemUser* user) : RB_ObjectContainer(user) {
	createMembers();
	*this = *user;
}


DB_SystemUser::~DB_SystemUser() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create object members
 * -firstname
 * -lastname
 * -email
 * -telephone
 * -username
 * -password
 * -start
 * -end
 */
void DB_SystemUser::createMembers() {
    addMember("firstname", "-", "<NEW>", RB2::MemberChar40);
    addMember("lastname", "-", "", RB2::MemberChar125);
    addMember("email", "-", "", RB2::MemberChar125);
    addMember("telephone", "-", "", RB2::MemberChar40);
    addMember("username", "-", "", RB2::MemberChar40);
    addMember("password", "-", "", RB2::MemberChar255);
    addMember("start", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
    addMember("end", "-", "1970-01-01T00:00:00", RB2::MemberDateTime);
}
