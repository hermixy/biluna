/*****************************************************************
 * $Id: db_testproject.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Sep 1, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testproject.h"


DB_TestProject::DB_TestProject (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


DB_TestProject::DB_TestProject(DB_TestProject* project) : RB_ObjectContainer(project) {
    createMembers();
    *this = *project;
}


DB_TestProject::~DB_TestProject() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_TestProject::createMembers() {
    addMember("number", "-", "New Project", RB2::MemberChar125);
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("description", "-", "", RB2::MemberChar125);
    addMember("company", "-", "", RB2::MemberChar125);
    addMember("location", "-", "", RB2::MemberChar125);
}
