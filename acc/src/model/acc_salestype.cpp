/*****************************************************************
 * $Id: acc_salestype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salestype.h"

ACC_SalesType::ACC_SalesType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SalesType::ACC_SalesType(ACC_SalesType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SalesType::~ACC_SalesType() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - typeabbrev sales type abbreviation
 * - description sales type description
 */
void ACC_SalesType::createMembers() {
    addMember("typeabbrev", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
}
