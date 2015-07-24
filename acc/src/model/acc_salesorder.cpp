/*****************************************************************
 * $Id: acc_salesorder.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Jan 23, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesorder.h"

ACC_SalesOrder::ACC_SalesOrder (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SalesOrder::ACC_SalesOrder(ACC_SalesOrder* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SalesOrder::~ACC_SalesOrder() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - orderno order number
 * - customerref customer reference number
 * - buyername customer buyer (salesman) name
 * - ordertitle title of order
 * - comments additional comments to this order
 * - orddate date of order
 * - ordertype type of order
 * - shipvia_id shipment via DHL or TNT ID
 * - deladd1 delivery address line 1 - 6
 * - contact_idx contact from contact list
 * - contactphone alternative contact phone number
 * - contactemail alternative contact email address
 * - deliverto deliver to, alternative company name
 * - deliverblind deliver blind, e.g. no company details
 * - freightcost freight cost
 * - fromstkloc_id from stock location dispatched
 * - deliverydate delivery due date
 * - quotedate quotation date
 * - confirmeddate order confirmation date
 * - printedpackingslip 1 if packing slip was printed
 * - datepackingslipprinted date when packing slip was printed
 * - quotation 1 if this is only a quotation
 * - invoicecomment extra comment to be shown on the invoice
 * - consignment reference to consignment note, applicable if goods are delivered by consignment
 */
void ACC_SalesOrder::createMembers() {
    addMember(this, "orderno", "-", 0, RB2::MemberInteger);
    addMember(this, "customerref", "-", "", RB2::MemberChar125);
    addMember(this, "buyername", "-", "", RB2::MemberChar125);
    addMember(this, "ordertitle", "-", "", RB2::MemberChar125);
    addMember(this, "comments", "-", "", RB2::MemberString);
    addMember(this, "orddate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember(this, "ordertype", "-", "", RB2::MemberChar40);
    addMember(this, "shipvia_id", "-", "0", RB2::MemberChar40);
    addMember(this, "deladd1", "-", "", RB2::MemberChar125);
    addMember(this, "deladd2", "-", "", RB2::MemberChar125);
    addMember(this, "deladd3", "-", "", RB2::MemberChar125);
    addMember(this, "deladd4", "-", "", RB2::MemberChar125);
    addMember(this, "deladd5", "-", "", RB2::MemberChar125);
    addMember(this, "deladd6", "-", "", RB2::MemberChar40);
    addMember(this, "contact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "contactphone", "-", "", RB2::MemberChar20);
    addMember(this, "contactemail", "-", "", RB2::MemberChar125);
    addMember(this, "deliverto", "-", "", RB2::MemberChar125); // alternative company name
    addMember(this, "deliverblind", "-", 0, RB2::MemberInteger); // no company details
    addMember(this, "freightcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "fromstkloc_id", "-", "0", RB2::MemberChar40);
    addMember(this, "deliverydate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember(this, "quotedate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember(this, "confirmeddate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember(this, "printedpackingslip", "-", 0, RB2::MemberInteger);
    addMember(this, "datepackingslipprinted", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
    addMember(this, "quotation", "-", 0, RB2::MemberInteger);
    // added
    // Extra comment to be shown on the invoice
    addMember(this, "invoicecomment", "-", "", RB2::MemberChar255);
    // Reference to consignment note, applicable if goods are delivered by consignment
    addMember(this, "consignment", "-", "", RB2::MemberChar20);
}
