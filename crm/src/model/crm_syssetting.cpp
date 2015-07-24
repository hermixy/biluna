/*****************************************************************
 * $Id: crm_syssetting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 16:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_syssetting.h"

CRM_SysSetting::CRM_SysSetting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


CRM_SysSetting::CRM_SysSetting(CRM_SysSetting* obj)
        : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


CRM_SysSetting::~CRM_SysSetting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/*!
  Create members:
  \li syssetingname system setting name (placeholder only)
 */
void CRM_SysSetting::createMembers() {
    addMember(this, "syssettingname", "-", "Placeholder only", RB2::MemberChar40);
}
