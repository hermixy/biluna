/*****************************************************************
 * $Id: acc_debtortype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 17, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_debtortype.h"


ACC_DebtorType::ACC_DebtorType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_DebtorType::ACC_DebtorType(ACC_DebtorType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_DebtorType::~ACC_DebtorType() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
 * Create members:
 * - typename type name
 */
void ACC_DebtorType::createMembers() {
    addMember(this, "typename", "-", "", RB2::MemberChar125);      // 3
}
