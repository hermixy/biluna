/*****************************************************************
 * $Id: sail_route.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_softwarelicense.h"

MRP_SoftwareLicense::MRP_SoftwareLicense (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


MRP_SoftwareLicense::MRP_SoftwareLicense(MRP_SoftwareLicense* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


MRP_SoftwareLicense::~MRP_SoftwareLicense() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li licensecode, license code
 * \li startdate, start date of the license
 * \li enddate, end date of the license
 * \li customer_idx, id and name of customer
 * \li salesorder_idx, id and number of sales order
 * \li contact_idx, id and name of contact person
 * \li comment, comment or remark with this code
 */
void MRP_SoftwareLicense::createMembers() {
    addMember(this, "licensecode", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "startdate", "-", "2000-01-01", RB2::MemberChar20);
    addMember(this, "enddate", "-", "2000-01-01", RB2::MemberChar20);
    addMember(this, "customer_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "salesorder_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "contact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "comment", "-", "", RB2::MemberChar125);
}
