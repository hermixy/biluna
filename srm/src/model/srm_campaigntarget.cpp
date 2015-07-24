/*****************************************************************
 * $Id: srm_campaigntarget.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 5, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_campaigntarget.h"

SRM_CampaignTarget::SRM_CampaignTarget (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SRM_CampaignTarget::SRM_CampaignTarget(SRM_CampaignTarget* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SRM_CampaignTarget::~SRM_CampaignTarget() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - group_idx campaign group ID and description
 * - note reference note with this group
 */
void SRM_CampaignTarget::createMembers() {
    addMember(this, "group_idx", "-", "0", RB2::MemberChar125);
    addMember(this, "note", "-", "<NEW>", RB2::MemberChar255);
}
