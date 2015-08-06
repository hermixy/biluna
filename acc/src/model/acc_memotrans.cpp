/*****************************************************************
 * $Id: acc_memotrans.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 13, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_memotrans.h"

ACC_MemoTrans::ACC_MemoTrans (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_MemoTrans::ACC_MemoTrans(ACC_MemoTrans* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_MemoTrans::~ACC_MemoTrans() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - transdoc_id ID of transaction document
 * - description description of this bank line item
 * - chartmaster_idx GL account ID and description
 * - accountcontrol account control
 * - amount this bank document item amount
 * - transno transaction number of the transaction document
 * - transallocn_idx transaction allocation ID and description
 */
void ACC_MemoTrans::createMembers() {
    addMember("transdoc_id", "-", "0", RB2::MemberChar40);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("chartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember("accountcontrol", "-", 0, RB2::MemberInteger);
    addMember("amount", "-", 0.0, RB2::MemberDouble);
    addMember("transno", "-", 0, RB2::MemberInteger);
    addMember("transallocn_idx", "-", "0", RB2::MemberChar165);
}
