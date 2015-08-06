/*****************************************************************
 * $Id: srm_campaigntype.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_campaigntype.h"

SRM_CampaignType::SRM_CampaignType (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SRM_CampaignType::SRM_CampaignType(SRM_CampaignType* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SRM_CampaignType::~SRM_CampaignType() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - mname name of campaign type
 */
void SRM_CampaignType::createMembers() {
    addMember("mname", "-", "<new>", RB2::MemberChar125);
}
