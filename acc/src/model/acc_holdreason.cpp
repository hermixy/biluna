/*****************************************************************
 * $Id: acc_holdreason.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_holdreason.h"

ACC_HoldReason::ACC_HoldReason (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_HoldReason::ACC_HoldReason(ACC_HoldReason* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_HoldReason::~ACC_HoldReason() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - reasoncode reason code usually capitals
 * - reasondescription description
 * - dissallowinvoices disallow invoices with this hold reason
 */
void ACC_HoldReason::createMembers() {
    addMember("reasoncode", "-", "0", RB2::MemberChar40);
    addMember("reasondescription", "-", "", RB2::MemberChar255);
    addMember("dissallowinvoices", "-", 0, RB2::MemberInteger);
}
