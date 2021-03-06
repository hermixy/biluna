/*****************************************************************
 * $Id: srm_leadsourcetype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_leadsourcetype.h"

SRM_LeadSourceType::SRM_LeadSourceType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SRM_LeadSourceType::SRM_LeadSourceType(SRM_LeadSourceType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SRM_LeadSourceType::~SRM_LeadSourceType() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li mname name of the lead source type
 */
void SRM_LeadSourceType::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar40);
}
