/*****************************************************************
 * $Id: crm_customerdetail.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 19, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_customerdetail.h"

/**
 * Constructor
 */
CRM_CustomerDetail::CRM_CustomerDetail (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


/**
 * Constructor
 */
CRM_CustomerDetail::CRM_CustomerDetail(CRM_CustomerDetail* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


/**
 * Destructor
 */
CRM_CustomerDetail::~CRM_CustomerDetail() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


/**
Create members.
Members are:
\li customertype - hardcoded: Analyst, Competitor, Customer, Integrator,
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
void CRM_CustomerDetail::createMembers() {
    addMember("customertype_id", "-", 0, RB2::MemberInteger);
    addMember("ownership", "-", "", RB2::MemberChar125);
    addMember("classifcode", "-", "", RB2::MemberChar125);
    addMember("industrytype_id", "-", 0, RB2::MemberInteger);
    addMember("employeecount", "-", 0, RB2::MemberInteger);
    addMember("annualrevenue", "-", "", RB2::MemberChar125);
    addMember("rating", "-", "", RB2::MemberChar125);
}
