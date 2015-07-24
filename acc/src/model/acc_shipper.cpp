/*****************************************************************
 * $Id: acc_shipper.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_shipper.h"

ACC_Shipper::ACC_Shipper (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Shipper::ACC_Shipper(ACC_Shipper* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_Shipper::~ACC_Shipper() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - shippername shipper name
 * - mincharge minimum or lowest charge for shipments
 */
void ACC_Shipper::createMembers() {
    addMember(this, "shippername", "-", "Default", RB2::MemberChar125);
    addMember(this, "mincharge", "-", 0.0, RB2::MemberDouble);
}
