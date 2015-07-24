/*****************************************************************
 * $Id: acc_factorcompany.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 18, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_factorcompany.h"

ACC_FactorCompany::ACC_FactorCompany (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_FactorCompany::ACC_FactorCompany(ACC_FactorCompany* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_FactorCompany::~ACC_FactorCompany() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - coyname factor company name
 * - address1 - 6 addresses
 * - contact contact name
 * - telephone telephone number
 * - fax fax number
 * - email email address
 */
void ACC_FactorCompany::createMembers() {
    addMember(this, "coyname", "-", "", RB2::MemberChar125);
    addMember(this, "address1", "-", "", RB2::MemberChar125);
    addMember(this, "address2", "-", "", RB2::MemberChar125);
    addMember(this, "address3", "-", "", RB2::MemberChar125);
    addMember(this, "address4", "-", "", RB2::MemberChar125);
    addMember(this, "address5", "-", "", RB2::MemberChar125);
    addMember(this, "address6", "-", "", RB2::MemberChar40);
    addMember(this, "contact", "-", "", RB2::MemberChar125);
    addMember(this, "telephone", "-", "", RB2::MemberChar20);
    addMember(this, "fax", "-", "", RB2::MemberChar20);
    addMember(this, "email", "-", "", RB2::MemberChar125);
}
