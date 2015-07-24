/*****************************************************************
 * $Id: acc_glsum.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 6, 2010 8:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_glsum.h"

ACC_GlSum::ACC_GlSum (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_GlSum::ACC_GlSum(ACC_GlSum* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_GlSum::~ACC_GlSum() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - period is month in year for example June in 2012 is 201206
 * - accountcode GL account code or number
 * - accountname name of the GL account
 * - debit debit amount
 * - credit credit amount
 */
void ACC_GlSum::createMembers() {
    addMember(this, "period", "-", 0, RB2::MemberInteger);
    addMember(this, "accountcode", "-", 0, RB2::MemberInteger);
    addMember(this, "accountname", "-", 0, RB2::MemberChar255);
    addMember(this, "debit", "-", 0.0, RB2::MemberDouble);
    addMember(this, "credit", "-", 0.0, RB2::MemberDouble);
}
