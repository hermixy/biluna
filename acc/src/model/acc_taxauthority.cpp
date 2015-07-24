/*****************************************************************
 * $Id: acc_taxauthority.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 22, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxauthority.h"


ACC_TaxAuthority::ACC_TaxAuthority (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TaxAuthority::ACC_TaxAuthority(ACC_TaxAuthority* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TaxAuthority::~ACC_TaxAuthority() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - description description of this authority
 * - bank bank name
 * - bankacctype bank account type cheque or giro
 * - bankacc bank account number
 * - bankswift bank BIC or Swift number
 */
void ACC_TaxAuthority::createMembers() {
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "bank", "-", "", RB2::MemberChar125);
    addMember(this, "bankacctype", "-", "", RB2::MemberChar20);
    addMember(this, "bankacc", "-", "", RB2::MemberChar125);
    addMember(this, "bankswift", "-", "", RB2::MemberChar40);
}
