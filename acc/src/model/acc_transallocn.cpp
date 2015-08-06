/*****************************************************************
 * $Id: acc_transallocn.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Sep 14, 2010 17:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_transallocn.h"


ACC_TransAllocn::ACC_TransAllocn (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TransAllocn::ACC_TransAllocn(ACC_TransAllocn* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TransAllocn::~ACC_TransAllocn() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - amount amount of allocation
 * - datealloc date of allocation
 * - docfrom_id bank or journal document ID with allocated item
 * - itemallocfrom_id item ID of bank or journaldocument from which the allocation originates
 * - docallocto_id supplier or customer document ID to which the amount is allocated
 */
void ACC_TransAllocn::createMembers() {
    addMember("amount", "-", 0.0, RB2::MemberDouble);
    addMember("datealloc", "-", "2000-01-01 00:00:00", RB2::MemberDateTime);
    addMember("docfrom_id", "-", "", RB2::MemberChar40);
    addMember("itemallocfrom_id", "-", "", RB2::MemberChar40);
    addMember("docallocto_id", "-", "", RB2::MemberChar40);
}
