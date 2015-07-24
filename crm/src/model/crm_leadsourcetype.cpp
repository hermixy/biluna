/*****************************************************************
 * $Id: crm_leadsourcetype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_leadsourcetype.h"

CRM_LeadSourceType::CRM_LeadSourceType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


CRM_LeadSourceType::CRM_LeadSourceType(CRM_LeadSourceType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


CRM_LeadSourceType::~CRM_LeadSourceType() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname name of the lead source type
 */
void CRM_LeadSourceType::createMembers() {
    addMember(this, "mname", "-", "", RB2::MemberChar40);
}
