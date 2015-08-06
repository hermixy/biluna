/*****************************************************************
 * $Id: scan_project.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_project.h"


SCAN_Project::SCAN_Project (const RB_String& id, RB_ObjectBase* p,
                            const RB_String& n, RB_ObjectFactory* f)
                                : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SCAN_Project::SCAN_Project(SCAN_Project* project) : RB_ObjectContainer(project) {
    createMembers();
    *this = *project;
}


SCAN_Project::~SCAN_Project() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void SCAN_Project::createMembers() {
    addMember("number", "-", "NR0001", RB2::MemberChar125);
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("description", "-", "Default", RB2::MemberChar125);
    addMember("company", "-", "YourCompany", RB2::MemberChar125);
    addMember("location", "-", "YourLocation", RB2::MemberChar125);
}
