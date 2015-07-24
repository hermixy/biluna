/*****************************************************************
 * $Id: srm_contactdetail.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 1, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_contactdetail.h"

/**
 * Constructor
 */
SRM_ContactDetail::SRM_ContactDetail (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


/**
 * Constructor
 */
SRM_ContactDetail::SRM_ContactDetail(SRM_ContactDetail* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


/**
 * Destructor
 */
SRM_ContactDetail::~SRM_ContactDetail() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
Create members.
Members are:
\li leadsource_id - such as: Cold Call, Existing Customer, Self Generated,
Employee, Partner, Public Relations, Direct Mail Conference, Trade Show,
Web Site, Word of mouth, Email, Campaign, Other
\li birthday
\li spousname
\li childrennames
\li hobbies
\li background
 */
void SRM_ContactDetail::createMembers() {
    addMember(this, "leadsource_id", "-", "0", RB2::MemberChar40);
    addMember(this, "birthday", "-", "0000-00-00", RB2::MemberChar20);
    addMember(this, "spousname", "-", "", RB2::MemberChar125);
    addMember(this, "childrennames", "-", "", RB2::MemberChar125);
    addMember(this, "hobbies", "-", "", RB2::MemberChar125);
    addMember(this, "background", "-", "", RB2::MemberChar255);
}
