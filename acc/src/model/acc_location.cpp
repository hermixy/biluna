/*****************************************************************
 * $Id: acc_location.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 18, 2010 5:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_location.h"

ACC_Location::ACC_Location (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Location::ACC_Location(ACC_Location* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_Location::~ACC_Location() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - locationname location name
 * - deladd1 - 6 delivery address lines
 * - tel telephone number
 * - fax fax number
 * - email email address
 * - contact contact name
 * - taxprovince_id tax location ID
 * - managed 1 is managed inventory
 */
void ACC_Location::createMembers() {
    addMember(this, "locationname", "-", "", RB2::MemberChar125);
    addMember(this, "deladd1", "-", "", RB2::MemberChar125);
    addMember(this, "deladd2", "-", "", RB2::MemberChar125);
    addMember(this, "deladd3", "-", "", RB2::MemberChar125);
    addMember(this, "deladd4", "-", "", RB2::MemberChar125);
    addMember(this, "deladd5", "-", "", RB2::MemberChar125);
    addMember(this, "deladd6", "-", "", RB2::MemberChar40);
    addMember(this, "tel", "-", "", RB2::MemberChar20);
    addMember(this, "fax", "-", "", RB2::MemberChar20);
    addMember(this, "email", "-", "", RB2::MemberChar125);
    addMember(this, "contact", "-", "", RB2::MemberChar125);
    addMember(this, "taxprovince_id", "-", "0", RB2::MemberChar40);
    addMember(this, "managed", "-", 0, RB2::MemberInteger);
}
