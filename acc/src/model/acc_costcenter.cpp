/*****************************************************************
 * $Id: acc_costcenter.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jun 20, 2012 6:04:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costcenter.h"


ACC_CostCenter::ACC_CostCenter (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CostCenter::ACC_CostCenter(ACC_CostCenter* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CostCenter::~ACC_CostCenter() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li centercode code or number of the cost center
 * \li centername name of the cost center
 * \li description description of the cost center
 * \li centergroup_id center group ID
 */
void ACC_CostCenter::createMembers() {
    addMember("centercode", "-", "", RB2::MemberChar40);
    addMember("centername", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("centergroup_idx", "-", "0", RB2::MemberChar165);
}
