/*****************************************************************
 * $Id: acc_purchdata.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: May 26, 2010 6:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchdata.h"

ACC_PurchData::ACC_PurchData (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_PurchData::ACC_PurchData(ACC_PurchData* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_PurchData::~ACC_PurchData() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - stockmaster_idx stock item ID and description
 * - supplier_partno part number as used by supplier
 * - supplierdescription description of supplier
 * - price supplier price for the stock item
 * - supplieruom supplier unit of measurement
 * - conversionfactor conversion factor to company unit of measurement
 * - leadtime lead time, time from order to actual delivery
 * - preferred 1 if this is preferred supplier
 * - effectivefrom date from when the supplier is used
 */
void ACC_PurchData::createMembers() {
    addMember(this, "stockmaster_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "supplier_partno", "-", "", RB2::MemberChar125);
    addMember(this, "supplierdescription", "-", "", RB2::MemberChar255);
    addMember(this, "price", "-", 0.0, RB2::MemberDouble);
    addMember(this, "supplieruom", "-", "-", RB2::MemberChar40);
    addMember(this, "conversionfactor", "-", 0.0, RB2::MemberDouble);
    addMember(this, "leadtime", "-", 0, RB2::MemberInteger);
    addMember(this, "preferred", "-", 0, RB2::MemberInteger);
    addMember(this, "effectivefrom", "-", "", RB2::MemberDateTime);
}
