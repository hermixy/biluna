/*****************************************************************
 * $Id: acc_chartmaster.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 24, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_chartmaster.h"

ACC_ChartMaster::ACC_ChartMaster (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_ChartMaster::ACC_ChartMaster(ACC_ChartMaster* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_ChartMaster::~ACC_ChartMaster() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - accountcode account code usually a number, e.g. 1600
 * - accountname name of the GL account, for bank including
 *    the bank account number for importing bank statements
 * - accountgroup_id ID of the account group this determines
 *    whether it is a Profit and Loss account or a Balance Sheet account
 * - accountrequired 1 is required, the GL account can not be deleted
 * - accountcontrol account control, system number how to control this
 *    GL account, such as:
 *    control = 1 account receivable control (Debtor) (cannot be deleted)
 *    control = 2 account payable control (Creditor) (cannot be deleted)
 *    control = 1000-1999 cash control (Cash accounts) (cannot be deleted)
 *    control = 2000-2999 bank accounts control (Bank accounts) (cannot be deleted)
 */
void ACC_ChartMaster::createMembers() {
    addMember(this, "accountcode", "-", "0", RB2::MemberChar10);
    addMember(this, "accountname", "-", "", RB2::MemberChar125);
    addMember(this, "accountgroup_id", "-", "0", RB2::MemberChar40);
    addMember(this, "accountrequired", "-", 0, RB2::MemberInteger);
    addMember(this, "accountcontrol", "-", 0, RB2::MemberInteger);
}
