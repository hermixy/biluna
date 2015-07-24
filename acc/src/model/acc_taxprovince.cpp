/*****************************************************************
 * $Id: acc_taxprovince.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 18, 2010 4:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxprovince.h"

ACC_TaxProvince::ACC_TaxProvince (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TaxProvince::ACC_TaxProvince(ACC_TaxProvince* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TaxProvince::~ACC_TaxProvince() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxprovincename name of tax province
 */
void ACC_TaxProvince::createMembers() {
    addMember(this, "taxprovincename", "-", "", RB2::MemberChar125);   // 3
}
