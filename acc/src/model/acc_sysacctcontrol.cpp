/*****************************************************************
 * $Id: acc_sysacctcontrol.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Nov 11, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_sysacctcontrol.h"

ACC_SysAcctControl::ACC_SysAcctControl (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SysAcctControl::ACC_SysAcctControl(ACC_SysAcctControl* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SysAcctControl::~ACC_SysAcctControl() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members
 * - controlnumber
 * - description
 * - taxcontrolno
 * controlnumber 0 is default, not a journal/account nor a sales tax account
 * controlnumber > 0 and < 1000 sales tax accounts or related to sales tax
 * controlnumber >= 1000 journal accounts and should allway be balanced
 * The  tax control number is the control number of the account
 * to which the sales/purchase tax should be booked. Note: the tax accounts
 * themselves of course do not have a taxcontrolno.
 */
void ACC_SysAcctControl::createMembers() {
    addMember("controlnumber", "-", 0, RB2::MemberInteger);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("taxcontrolno", "-", 0, RB2::MemberInteger);
}
