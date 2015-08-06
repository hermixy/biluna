/*****************************************************************
 * $Id: peng_project.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 14, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_project.h"


PENG_Project::PENG_Project (const RB_String& id, RB_ObjectBase* p,
                            const RB_String& n, RB_ObjectFactory* f)
                                : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


PENG_Project::PENG_Project(PENG_Project* project) : RB_ObjectContainer(project) {
    createMembers();
    *this = *project;
}


PENG_Project::~PENG_Project() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void PENG_Project::createMembers() {
    addMember("number", "-", "PENG0001", RB2::MemberChar125);
    addMember("revision", "-", 0, RB2::MemberInteger);
    addMember("description", "-", "Default", RB2::MemberChar125);
    addMember("company", "-", "YourCompany", RB2::MemberChar125);
    addMember("location", "-", "YourLocation", RB2::MemberChar125);
}
