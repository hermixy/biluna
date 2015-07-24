/*****************************************************************
 * $Id: srm_supplierdetail.cpp 2101 2014-02-19 11:55:29Z rutger $
 * Created: Feb 18, 2014 14:24:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_supplierdetail.h"

/**
 * Constructor
 */
SRM_SupplierDetail::SRM_SupplierDetail (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


/**
 * Constructor
 */
SRM_SupplierDetail::SRM_SupplierDetail(SRM_SupplierDetail* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


/**
 * Destructor
 */
SRM_SupplierDetail::~SRM_SupplierDetail() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
Create members.
Members are:
\li suppliertype - hardcoded: Analyst, Competitor, Supplier, Integrator,
Investor, Partner, Press, Prospect, Reseller, Other
\li ownership
\li classifcode - in USA this would be the SIC code (Standard Industrial Code)
\li industrytype - hardcoded: Apparel, Banking, Biotechnology, Chemicals,
Communications, Construction, Consulting, Education, Electronics, Energy,
Engineering, Entertainment, Environmental, Finance, Government, Healthcare,
Hospitality, Insurance, Machinery, Manufacturing, Media, Not For Profit,
Recreation, Retail, Shipping, Technology, Telecommunications, Transportation
Utilities, Other
\li employeecount
\li annualrevenue
\li rating

 */
void SRM_SupplierDetail::createMembers() {
    addMember(this, "suppliertype_id", "-", 0, RB2::MemberInteger);
    addMember(this, "ownership", "-", "", RB2::MemberChar125);
    addMember(this, "classifcode", "-", "", RB2::MemberChar125);
    addMember(this, "industrytype_id", "-", 0, RB2::MemberInteger);
    addMember(this, "employeecount", "-", 0, RB2::MemberInteger);
    addMember(this, "annualrevenue", "-", "", RB2::MemberChar125);
    addMember(this, "rating", "-", "", RB2::MemberChar125);
}
