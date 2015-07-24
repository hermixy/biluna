/*****************************************************************
 * $Id: acc_supplierattr.cpp 2099 2014-02-17 19:48:13Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_supplierattr.h"


ACC_SupplierAttr::ACC_SupplierAttr (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SupplierAttr::ACC_SupplierAttr(ACC_SupplierAttr* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SupplierAttr::~ACC_SupplierAttr() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - shortname name for supplier attribute
 * - description description of this attribute
 * - type type of attribute
 * - unit unit of measurement
 * - weight weight factor
 * - value of the attribute
 */
void ACC_SupplierAttr::createMembers() {
    addMember(this, "shortname", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "description", "-", "<New>", RB2::MemberChar255);
    addMember(this, "type", "-", 0, RB2::MemberInteger);
    addMember(this, "unit", "-", "", RB2::MemberChar40);
    addMember(this, "weight", "-", 0.0, RB2::MemberDouble);
    addMember(this, "value", "-", 0.0, RB2::MemberDouble);
}
