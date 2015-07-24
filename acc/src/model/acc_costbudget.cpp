/*****************************************************************
 * $Id: acc_costbudget.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jun 20, 2012 6:04:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costbudget.h"


ACC_CostBudget::ACC_CostBudget (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CostBudget::ACC_CostBudget(ACC_CostBudget* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CostBudget::~ACC_CostBudget() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li period period in year month, e.g. 201206 is June 2012
 * \li note comment note to this budget
 * \li budget budget amount for this period
 * \li forecast forecasted amount for this period
 */
void ACC_CostBudget::createMembers() {
    addMember(this, "period", "-", 0, RB2::MemberInteger);
    addMember(this, "note", "-", "", RB2::MemberChar255);
    addMember(this, "budget", "-", 0.0, RB2::MemberDouble);
    addMember(this, "forecast", "-", 0.0, RB2::MemberDouble);
}
