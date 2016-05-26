/*****************************************************************
 * $Id: db_test.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Sep 2, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_test.h"

DB_Test::DB_Test (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


DB_Test::DB_Test(DB_Test* test) : RB_ObjectContainer(test) {
    createMembers();
    *this = *test;
}


DB_Test::~DB_Test() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_Test::createMembers() {
    // RB_String uuid = "";
    addMember("value1", "-", "New Test", RB2::MemberChar125);
    addMember("value2", "mm", 0.0, RB2::MemberDouble);
    addMember("value3", "mm", 0.0, RB2::MemberDouble);
    // Number in combobox (or text of combobox)
    addMember("combobox1", "-", 0, RB2::MemberInteger);
    // Dynamic model, only holds the reference (Uuid) of the relational table
    addMember("combobox2", "-", "0", RB2::MemberChar40);
    addMember("spinbox", "-", 0, RB2::MemberInteger);
    addMember("datetimeedit", "-", "", RB2::MemberDateTime);
    // Text of checkbox true or false
    addMember("checkbox", "-", "", RB2::MemberChar40);
    // ID plus text, only to be used in special cases
    addMember("selectrel_idx", "-", "", RB2::MemberChar255);
    addMember("text", "-", "Text ...", RB2::MemberChar125);
}
