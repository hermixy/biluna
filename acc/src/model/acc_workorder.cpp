/*****************************************************************
 * $Id: acc_workorder.cpp 2191 2014-10-27 20:31:51Z rutger $
 * Created: Oct 27, 2014 11:41:25 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_workorder.h"

ACC_WorkOrder::ACC_WorkOrder (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_WorkOrder::ACC_WorkOrder(ACC_WorkOrder* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_WorkOrder::~ACC_WorkOrder() {
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
void ACC_WorkOrder::createMembers() {
    return;
    // TODO

    addMember("orderno", "-", 0, RB2::MemberInteger);
    addMember("customerref", "-", "", RB2::MemberChar125);
    addMember("buyername", "-", "", RB2::MemberChar125);
    addMember("ordertitle", "-", "", RB2::MemberChar125);
    addMember("comments", "-", "", RB2::MemberString);
    addMember("orddate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember("ordertype", "-", "", RB2::MemberChar40);
    addMember("shipvia_id", "-", "0", RB2::MemberChar40);
    addMember("deladd1", "-", "", RB2::MemberChar125);
    addMember("deladd2", "-", "", RB2::MemberChar125);
    addMember("deladd3", "-", "", RB2::MemberChar125);
    addMember("deladd4", "-", "", RB2::MemberChar125);
    addMember("deladd5", "-", "", RB2::MemberChar125);
    addMember("deladd6", "-", "", RB2::MemberChar40);
    addMember("contact_idx", "-", "0", RB2::MemberChar165);
    addMember("contactphone", "-", "", RB2::MemberChar20);
    addMember("contactemail", "-", "", RB2::MemberChar125);
    addMember("deliverto", "-", "", RB2::MemberChar125); // alternative company name
    addMember("deliverblind", "-", 0, RB2::MemberInteger); // no company details
    addMember("freightcost", "-", 0.0, RB2::MemberDouble);
    addMember("fromstkloc_id", "-", "0", RB2::MemberChar40);
    addMember("deliverydate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember("quotedate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember("confirmeddate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember("printedpackingslip", "-", 0, RB2::MemberInteger);
    addMember("datepackingslipprinted", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
    addMember("quotation", "-", 0, RB2::MemberInteger);
    // added
    // Extra comment to be shown on the invoice
    addMember("invoicecomment", "-", "", RB2::MemberChar255);
    // Reference to consignment note, applicable if goods are delivered by consignment
    addMember("consignment", "-", "", RB2::MemberChar20);
}
