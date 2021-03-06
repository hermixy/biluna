/*****************************************************************
 * $Id: acc_assetcustattr.cpp 1677 2012-06-20 20:32:29Z rutger $
 * Created: Sept 18, 2012 1:42:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_supplcostpart.h"


ACC_SupplCostPart::ACC_SupplCostPart (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


ACC_SupplCostPart::ACC_SupplCostPart(ACC_SupplCostPart* branch) : RB_ObjectAtomic(branch) {
    createMembers();
    *this = *branch;
}


ACC_SupplCostPart::~ACC_SupplCostPart() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - shortname is the name for this cost center part
 * - description is the description of this cost center part
 * - type is the budget, forecast or actual value of the cost center
 * - weight is in percentage the part of the cost center value
 * - startdate is the validity startdate of this cost center part
 * - enddate is the validity enddate of this cost center part
 * - costcenter_idx is the ID of the cost center
 */
void ACC_SupplCostPart::createMembers() {
    addMember("shortname", "-", "<NEW>", RB2::MemberChar40);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("type_id", "-", 0, RB2::MemberInteger);
    addMember("weight", "-", 0.0, RB2::MemberDouble);
    addMember("startdate", "-", "", RB2::MemberChar20);
    addMember("enddate", "-", "", RB2::MemberChar20);
    addMember("costgroup_idx", "-", "0", RB2::MemberChar165);
    addMember("costcenter_idx", "-", "0", RB2::MemberChar165);
}
