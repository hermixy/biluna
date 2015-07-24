/*****************************************************************
 * $Id: sail_project.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_project.h"

SAIL_Project::SAIL_Project (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SAIL_Project::SAIL_Project(SAIL_Project* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SAIL_Project::~SAIL_Project() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li code project code
 * \li description description of this SAIL project
 */
void SAIL_Project::createMembers() {
    addMember(this, "code", "-", "<NEW>", RB2::MemberChar20);
    addMember(this, "description", "-", "", RB2::MemberChar125);
}
