/*****************************************************************
 * $Id: acc_taxgroup.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxgroup.h"


ACC_TaxGroup::ACC_TaxGroup (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TaxGroup::ACC_TaxGroup(ACC_TaxGroup* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TaxGroup::~ACC_TaxGroup() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxgroupdescr tax location name/description
 */
void ACC_TaxGroup::createMembers() {
    addMember(this, "taxgroupdescr", "-", "", RB2::MemberChar125);
}
