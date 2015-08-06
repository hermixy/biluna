/*****************************************************************
 * $Id: srm_group.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_group.h"

SRM_Group::SRM_Group (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SRM_Group::SRM_Group(SRM_Group* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SRM_Group::~SRM_Group() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname name of the group
 * \li description of the group
 */
void SRM_Group::createMembers() {
    addMember("mname", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
}
