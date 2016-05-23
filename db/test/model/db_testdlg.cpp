/*****************************************************************
 * $Id: db_testdlg.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Oct 26, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testdlg.h"

DB_TestDlg::DB_TestDlg (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


DB_TestDlg::DB_TestDlg(DB_TestDlg* testDlg) : RB_ObjectAtomic(testDlg) {
    createMembers();
    *this = *testDlg;
}


DB_TestDlg::~DB_TestDlg() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_TestDlg::createMembers() {
    addMember("value1", "-", "New TestDlg", RB2::MemberChar125);
    addMember("value2", "mm", 0.0, RB2::MemberDouble);
}
