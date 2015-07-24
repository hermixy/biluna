/*****************************************************************
 * $Id: srm_activity.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_activity.h"

SRM_Activity::SRM_Activity (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SRM_Activity::SRM_Activity(SRM_Activity* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SRM_Activity::~SRM_Activity() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - activitytype_id activity type ID
 * - activitycode code by user for easy reference of activity, usually capitals
 * - subject subject of activity
 * - mstart start date/time of activity
 * - mend end date/time of activity
 * - isreminder 1 if this is reminder for later date
 * - remindertime time when reminder is activated
 * - priority_id priority ID such as none, low, medium and high
 * - description description of activity
 * - status_id status ID such as not started, ongoing and completed
 * - location location indicator where activity takes place
 * - mvalue money value in case of opportunity
 * - acc_contact_id customer contact ID (in ACC)
 * - acc_customer_id cusomer ID (in ACC)
 */
void SRM_Activity::createMembers() {
    addMember(this, "activitytype_id", "-", 0, RB2::MemberInteger);
    addMember(this, "activitycode", "-", "", RB2::MemberChar40);
    addMember(this, "subject", "-", "", RB2::MemberChar125);
    addMember(this, "mstart", "-", "", RB2::MemberDateTime);
    addMember(this, "mend", "-", "", RB2::MemberDateTime);
    addMember(this, "isreminder", "-", 0, RB2::MemberInteger);
    addMember(this, "remindertime", "day", 0, RB2::MemberInteger);
    addMember(this, "priority_id", "-", 0, RB2::MemberInteger);
    addMember(this, "description", "-", "", RB2::MemberChar2500);
    addMember(this, "status_id", "-", 0, RB2::MemberInteger);
    addMember(this, "location", "-", "", RB2::MemberChar125);
    addMember(this, "mvalue", "currency", 0.0, RB2::MemberDouble);
    addMember(this, "acc_contact_id", "-", "0", RB2::MemberChar40);
    addMember(this, "acc_supplier_id", "-", "0", RB2::MemberChar40);
}
