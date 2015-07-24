/*****************************************************************
 * $Id: acc_taxcategory.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 22, 2010 9:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxcategory.h"

ACC_TaxCategory::ACC_TaxCategory (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TaxCategory::ACC_TaxCategory(ACC_TaxCategory* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TaxCategory::~ACC_TaxCategory() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxcatname tax category name
 */
void ACC_TaxCategory::createMembers() {
    addMember(this, "taxcatname", "-", "", RB2::MemberChar125);      // 3
}
