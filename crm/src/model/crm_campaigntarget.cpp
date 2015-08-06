/*****************************************************************
 * $Id: crm_campaigntarget.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_campaigntarget.h"

CRM_CampaignTarget::CRM_CampaignTarget (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


CRM_CampaignTarget::CRM_CampaignTarget(CRM_CampaignTarget* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


CRM_CampaignTarget::~CRM_CampaignTarget() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - group_idx campaign group ID and description
 * - note reference note with this group
 */
void CRM_CampaignTarget::createMembers() {
    addMember("group_idx", "-", "0", RB2::MemberChar125);
    addMember("note", "-", "<NEW>", RB2::MemberChar255);
}
