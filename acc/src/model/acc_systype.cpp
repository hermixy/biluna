/*****************************************************************
 * $Id: acc_systype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_systype.h"


ACC_SysType::ACC_SysType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SysType::ACC_SysType(ACC_SysType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SysType::~ACC_SysType() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Members are:
 * - systypecode integer number as per ACC2::TransType
 * - systypename name of system type for user reference
 * - systypeno sequence number of the system typedef
 * - systypelist_id ID of the system type list for example the ACC_ChartMaster ID
 */
void ACC_SysType::createMembers() {
    addMember(this, "systypecode", "-", 0, RB2::MemberInteger);
    addMember(this, "systypename", "-", "", RB2::MemberChar125);
    addMember(this, "systypeno", "-", 0, RB2::MemberInteger);
    addMember(this, "systypelist_id", "-", "0", RB2::MemberChar40);
}
