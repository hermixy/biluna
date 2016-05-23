/*****************************************************************
 * $Id: db_testchild.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Sep 1, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testchild.h"

DB_TestChild::DB_TestChild (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


DB_TestChild::DB_TestChild(DB_TestChild* child) : RB_ObjectContainer(child) {
    createMembers();
    *this = *child;
}


DB_TestChild::~DB_TestChild() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_TestChild::createMembers() {
    addMember("number", "-", "New Child", RB2::MemberChar125);
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("description", "-", "", RB2::MemberChar125);
    addMember("company", "-", "", RB2::MemberChar125);
    addMember("location", "-", "", RB2::MemberChar125);
}
