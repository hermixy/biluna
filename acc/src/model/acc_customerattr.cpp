/*****************************************************************
 * $Id: acc_customerattr.cpp 2099 2014-02-17 19:48:13Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_customerattr.h"


ACC_CustomerAttr::ACC_CustomerAttr (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CustomerAttr::ACC_CustomerAttr(ACC_CustomerAttr* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CustomerAttr::~ACC_CustomerAttr() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - shortname name for customer attribute
 * - description description of this attribute
 * - type type of attribute
 * - unit unit of measurement
 * - weight weight factor
 * - value of the attribute
 */
void ACC_CustomerAttr::createMembers() {
    addMember("shortname", "-", "<NEW>", RB2::MemberChar125);
    addMember("description", "-", "<New>", RB2::MemberChar255);
    addMember("type", "-", 0, RB2::MemberInteger);
    addMember("unit", "-", "", RB2::MemberChar40);
    addMember("weight", "-", 0.0, RB2::MemberDouble);
    addMember("value", "-", 0.0, RB2::MemberDouble);
}
