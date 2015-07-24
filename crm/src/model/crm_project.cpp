/*****************************************************************
 * $Id: crm_project.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_project.h"

CRM_Project::CRM_Project (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


CRM_Project::CRM_Project(CRM_Project* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


CRM_Project::~CRM_Project() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li code account code
 * \li description description of this CRM account
 * \li user_idx user ID and description
 */
void CRM_Project::createMembers() {
    addMember(this, "code", "-", "<NEW>", RB2::MemberChar20);
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "user_idx", "-", "0", RB2::MemberChar125);
}
