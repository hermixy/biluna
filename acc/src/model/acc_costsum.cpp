/*****************************************************************
 * $Id: acc_costsum.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jun 24, 2012 1:50:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costsum.h"

ACC_CostSum::ACC_CostSum (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


ACC_CostSum::ACC_CostSum(ACC_CostSum* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


ACC_CostSum::~ACC_CostSum() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - period is month in year for example June in 2012 is 201206
 * - centercode cost/profit center code or number
 * - centername name of cost/profit center
 * - debit actual debit amount
 * - credit actual credit amount
 */
void ACC_CostSum::createMembers() {
    addMember(this, "period", "-", 0, RB2::MemberInteger);
    addMember(this, "centercode", "-", "", RB2::MemberChar40);
    addMember(this, "centername", "-", "", RB2::MemberChar255);
    addMember(this, "debit", "-", 0.0, RB2::MemberDouble);
    addMember(this, "credit", "-", 0.0, RB2::MemberDouble);
}
