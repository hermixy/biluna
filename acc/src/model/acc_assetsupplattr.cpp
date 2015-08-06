/*****************************************************************
 * $Id: acc_assetsupplattr.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assetsupplattr.h"


ACC_AssetSupplAttr::ACC_AssetSupplAttr (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_AssetSupplAttr::ACC_AssetSupplAttr(ACC_AssetSupplAttr* branch) : RB_ObjectContainer(branch) {
    createMembers();
    *this = *branch;
}


ACC_AssetSupplAttr::~ACC_AssetSupplAttr() {
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
void ACC_AssetSupplAttr::createMembers() {
    addMember("seqno", "-", 0, RB2::MemberInteger);
    addMember("shortname", "-", "<NEW>", RB2::MemberChar40);
    addMember("description", "-", "<New>", RB2::MemberChar255);
    addMember("unit", "-", "", RB2::MemberChar40);
    addMember("weight", "-", 0.0, RB2::MemberDouble);
    addMember("value", "-", 0.0, RB2::MemberDouble);
    addMember("multiplywith", "-", "", RB2::MemberChar40);
    addMember("startdate", "-", "", RB2::MemberChar20);
    addMember("enddate", "-", "", RB2::MemberChar20);
    addMember("interval", "-", 0, RB2::MemberInteger);
    addMember("resultvalue", "-", 0.0, RB2::MemberDouble);
    addMember("supplier_idx", "-", "0", RB2::MemberChar165);}
