/*****************************************************************
 * $Id: acc_accountgroup.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 22, 2010 6:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_accountgroup.h"


ACC_AccountGroup::ACC_AccountGroup (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_AccountGroup::ACC_AccountGroup(ACC_AccountGroup* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_AccountGroup::~ACC_AccountGroup() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - groupname name of the account group
 * - pandl 0 is balance group and 1 is profit and loss group of accounts
 * - sequenceintb sequence number in trial balance
 */
void ACC_AccountGroup::createMembers() {
    addMember(this, "groupname", "-", "", RB2::MemberChar40);
    addMember(this, "pandl", "-", 0, RB2::MemberInteger);
    addMember(this, "sequenceintb", "-", 0, RB2::MemberInteger);
}
