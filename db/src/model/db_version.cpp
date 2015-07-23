/*****************************************************************
 * $Id: db_version.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Sept 2, 2013 10:00:42 AM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_version.h"

DB_Version::DB_Version (const RB_String& id, RB_ObjectBase *p,
                    const RB_String& n, RB_ObjectFactory *f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}


DB_Version::DB_Version(DB_Version* user) : RB_ObjectAtomic(user) {
	createMembers();
	*this = *user;
}


DB_Version::~DB_Version() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_Version::createMembers() {
    addMember(this, "perspectivecode", "-", "", RB2::MemberChar10);
    addMember(this, "major", "-", 0, RB2::MemberInteger);
    addMember(this, "medior", "-", 0, RB2::MemberInteger);
    addMember(this, "minor", "-", 0, RB2::MemberInteger);
    addMember(this, "build", "-", 0, RB2::MemberInteger);
    addMember(this, "comment", "-", "", RB2::MemberChar125);
}
