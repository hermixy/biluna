/*****************************************************************
 * $Id: acc_customernote.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Nov 14, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_customernote.h"

ACC_CustomerNote::ACC_CustomerNote (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CustomerNote::ACC_CustomerNote(ACC_CustomerNote* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CustomerNote::~ACC_CustomerNote() {
    // nothing
}

/**
 * Create members:
 * - subject subject of note
 * - description note description
 */
void ACC_CustomerNote::createMembers() {
    // conform webErp
    addMember("subject", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar2500);
}
