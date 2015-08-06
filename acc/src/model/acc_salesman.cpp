/*****************************************************************
 * $Id: acc_salesman.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 20, 2010 12:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesman.h"

ACC_SalesMan::ACC_SalesMan (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SalesMan::ACC_SalesMan(ACC_SalesMan* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SalesMan::~ACC_SalesMan() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - salesmanname salesman name
 * - smantel salesman telephone number
 * - smanfax salesman fax number
 * - commissionrate1 commission rate 1
 * - breakpoint breakpoint after which commission rate 2 is applicable
 * - commissionrate2 commission rate 2
 */
void ACC_SalesMan::createMembers() {
    addMember("salesmanname", "-", "", RB2::MemberChar125);
    addMember("smantel", "-", "", RB2::MemberChar20);
    addMember("smanfax", "-", "", RB2::MemberChar20);
    addMember("commissionrate1", "-", 0.0, RB2::MemberDouble);
    addMember("breakpoint", "-", 0.0, RB2::MemberDouble);
    addMember("commissionrate2", "-", 0.0, RB2::MemberDouble);
}
