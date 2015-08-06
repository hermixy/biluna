/*****************************************************************
 * $Id: crm_activity.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_activity.h"

CRM_Activity::CRM_Activity (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


CRM_Activity::CRM_Activity(CRM_Activity* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


CRM_Activity::~CRM_Activity() {
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
void CRM_Activity::createMembers() {
    addMember("activitytype_id", "-", 0, RB2::MemberInteger);
    addMember("activitycode", "-", "", RB2::MemberChar40);
    addMember("subject", "-", "", RB2::MemberChar125);
    addMember("mstart", "-", "", RB2::MemberDateTime);
    addMember("mend", "-", "", RB2::MemberDateTime);
    addMember("isreminder", "-", 0, RB2::MemberInteger);
    addMember("remindertime", "day", 0, RB2::MemberInteger);
    addMember("priority_id", "-", 0, RB2::MemberInteger);
    addMember("description", "-", "", RB2::MemberChar2500);
    addMember("status_id", "-", 0, RB2::MemberInteger);
    addMember("location", "-", "", RB2::MemberChar125);
    addMember("mvalue", "currency", 0.0, RB2::MemberDouble);
    addMember("acc_contact_id", "-", "0", RB2::MemberChar40);
    addMember("acc_customer_id", "-", "0", RB2::MemberChar40);
}
