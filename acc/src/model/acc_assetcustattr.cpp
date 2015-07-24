/*****************************************************************
 * $Id: acc_assetcustattr.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assetcustattr.h"


ACC_AssetCustAttr::ACC_AssetCustAttr (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_AssetCustAttr::ACC_AssetCustAttr(ACC_AssetCustAttr* branch) : RB_ObjectContainer(branch) {
    createMembers();
    *this = *branch;
}


ACC_AssetCustAttr::~ACC_AssetCustAttr() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - shortname is an abbreviation in capitles which can be used in multiplywith
 * - description is the description of this attribute
 * - seqno is the sequence number of the handling this attribute
 * - unit is the unit of measurement, in case of currency will be used in valuation
 * - delta is the percentage to be used of this attribute value or change in period
 * - multiplywith is the shortname resultvalue to be multiplied with this value
 * - startdate is the validity startdate of this attribute
 * - enddate is the validity enddate of this attribute
 * - interval is the time interval of occurence of this attribute
 * - resultvalue is the result value in case of multiplywith otherwise equal to value
 */
void ACC_AssetCustAttr::createMembers() {
    addMember(this, "seqno", "-", 0, RB2::MemberInteger);
    addMember(this, "shortname", "-", "<NEW>", RB2::MemberChar40);
    addMember(this, "description", "-", "<New>", RB2::MemberChar255);
    addMember(this, "unit", "-", "", RB2::MemberChar40);
    addMember(this, "weight", "-", 0.0, RB2::MemberDouble);
    addMember(this, "value", "-", 0.0, RB2::MemberDouble);
    addMember(this, "multiplywith", "-", "", RB2::MemberChar40);
    addMember(this, "startdate", "-", "", RB2::MemberChar20);
    addMember(this, "enddate", "-", "", RB2::MemberChar20);
    addMember(this, "interval", "-", 0, RB2::MemberInteger);
    addMember(this, "resultvalue", "-", 0.0, RB2::MemberDouble);
    addMember(this, "customer_idx", "-", "0", RB2::MemberChar165);
}
