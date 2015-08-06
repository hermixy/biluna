/*****************************************************************
 * $Id: acc_paymentterm.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 17, 2010 17:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_paymentterm.h"

ACC_PaymentTerm::ACC_PaymentTerm (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_PaymentTerm::ACC_PaymentTerm(ACC_PaymentTerm* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_PaymentTerm::~ACC_PaymentTerm() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - termsindicator code for the terms of payment
 * - terms payment terms and conditions
 * - daysbeforedue number of days after receipt of invoice
 * - dayinfollowingmonth 1 indicates that the previous daysbeforedue indicate
 *    the due day in the next month
 */
void ACC_PaymentTerm::createMembers() {
    // conform webErp
    addMember("termsindicator", "-", "", RB2::MemberChar40);
    addMember("terms", "-", "", RB2::MemberChar255);
    addMember("daysbeforedue", "-", 0, RB2::MemberInteger);
    addMember("dayinfollowingmonth", "-", 0, RB2::MemberInteger);
}
