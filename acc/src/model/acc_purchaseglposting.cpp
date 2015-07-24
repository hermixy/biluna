/*****************************************************************
 * $Id: acc_purchaseglposting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 28, 2013 17:17:25 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchaseglposting.h"


ACC_PurchaseGlPosting::ACC_PurchaseGlPosting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_PurchaseGlPosting::ACC_PurchaseGlPosting(ACC_PurchaseGlPosting* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_PurchaseGlPosting::~ACC_PurchaseGlPosting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxgroup_id tax group ID of the supplier
 * - stkcat_idx stock category of stock item sold
 * - purchaseglact_idx purchase GL account for this authority and stock category
 */
void ACC_PurchaseGlPosting::createMembers() {
    addMember(this, "taxgroup_id", "-", "0", RB2::MemberChar40);
    addMember(this, "stkcat_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "purchaseglact_idx", "-", "0", RB2::MemberChar165);
}
