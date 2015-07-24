/*****************************************************************
 * $Id: acc_suppliernote.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: May 23, 2012 10:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_suppliernote.h"

ACC_SupplierNote::ACC_SupplierNote (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SupplierNote::ACC_SupplierNote(ACC_SupplierNote* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SupplierNote::~ACC_SupplierNote() {
    // nothing
}

/**
 * Create members:
 * - subject subject of note
 * - description note description
 */
void ACC_SupplierNote::createMembers() {
    addMember(this, "subject", "-", "", RB2::MemberChar125);
    addMember(this, "description", "-", "", RB2::MemberChar2500);
}
