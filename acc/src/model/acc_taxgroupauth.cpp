/*****************************************************************
 * $Id: acc_taxgroupauth.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 24, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxgroupauth.h"


ACC_TaxGroupAuth::ACC_TaxGroupAuth (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TaxGroupAuth::ACC_TaxGroupAuth(ACC_TaxGroupAuth* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TaxGroupAuth::~ACC_TaxGroupAuth() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxauth_id tax authority ID
 * - calculationorder sequence number in order of tax calculation
 * - taxontax is 1 when tax on tax calculation
 */
void ACC_TaxGroupAuth::createMembers() {
    addMember("taxauth_id", "-", "0", RB2::MemberChar40);
    addMember("calculationorder", "-", 0, RB2::MemberInteger);
    addMember("taxontax", "-", 0, RB2::MemberInteger);
}
