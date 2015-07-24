/*****************************************************************
 * $Id: srm_groupcontact.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 05, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_groupcontact.h"

SRM_GroupContact::SRM_GroupContact (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SRM_GroupContact::SRM_GroupContact(SRM_GroupContact* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SRM_GroupContact::~SRM_GroupContact() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li contact_idx contact name ID and description
 * \li note comment note for ease of reference
 */
void SRM_GroupContact::createMembers() {
    addMember(this, "contact_idx", "-", "0", RB2::MemberChar125);
    addMember(this, "note", "-", "<NEW>", RB2::MemberChar125);
}
