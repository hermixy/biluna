/*****************************************************************
 * $Id: crm_group.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_group.h"

CRM_Group::CRM_Group (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


CRM_Group::CRM_Group(CRM_Group* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


CRM_Group::~CRM_Group() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname name of the group
 * \li description of the group
 */
void CRM_Group::createMembers() {
    addMember(this, "mname", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "description", "-", "", RB2::MemberChar255);
}
