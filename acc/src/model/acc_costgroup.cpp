/*****************************************************************
 * $Id: acc_costgroup.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jun 20, 2012 6:04:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costgroup.h"


ACC_CostGroup::ACC_CostGroup (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CostGroup::ACC_CostGroup(ACC_CostGroup* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CostGroup::~ACC_CostGroup() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li groupname name of the cost group
 * \li costprofit 0 is a costcenter group otherwise 1 a profitcenter group
 * \li sequenceno sequence number in report
 */
void ACC_CostGroup::createMembers() {
    addMember(this, "groupname", "-", "", RB2::MemberChar40);
    addMember(this, "costprofit", "-", 0, RB2::MemberInteger);
    addMember(this, "sequenceno", "-", 0, RB2::MemberInteger);
}
