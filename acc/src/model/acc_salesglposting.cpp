/*****************************************************************
 * $Id: acc_salesglposting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 02, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesglposting.h"


ACC_SalesGlPosting::ACC_SalesGlPosting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SalesGlPosting::ACC_SalesGlPosting(ACC_SalesGlPosting* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SalesGlPosting::~ACC_SalesGlPosting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxauth_id tax authority ID
 * - area_id sales area
 * - salestype_id sales type or price list used
 * - stkcat_idx stock category of stock item sold
 * - discountglact_idx discount GL account for this stock category
 * - salesglact_idx sales GL account for this stock category
 */
void ACC_SalesGlPosting::createMembers() {
    addMember(this, "taxauth_id", "-", "0", RB2::MemberChar40);
    addMember(this, "area_id", "-", "0", RB2::MemberChar40);
    addMember(this, "salestype_id", "-", "0", RB2::MemberChar40);
    addMember(this, "stkcat_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "discountglact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "salesglact_idx", "-", "0", RB2::MemberChar165);
}
