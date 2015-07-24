/*****************************************************************
 * $Id: acc_purchaseorder.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Sept 22, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchaseorder.h"

ACC_PurchaseOrder::ACC_PurchaseOrder (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_PurchaseOrder::ACC_PurchaseOrder(ACC_PurchaseOrder* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_PurchaseOrder::~ACC_PurchaseOrder() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - orderno order number
 * - requisitionno requisition number, technical specification
 * - receiptat_id receipt at warehouse of company ID
 * - orderdate date of order
 * - contact_idx contact from contact list
 * - contactphone alternative contact phone number
 * - contactemail alternative contact email address
 * - supplierref supplier reference number
 * - receiptdate due dat of receipt
 * - revision revision number of this order
 * - shipvia_id shipment via DHL or TNT ID
 * - orderstatus status of order for example 'on hold'
 * - quotation 1 is for quotation only
 * - deladd1 delivery address line 1 - 6
 * - ordertitle title of order
 * - comments additional comments to this order
 * - suppinvoiceno supplier invoice number
 * - suppinvoicedate supplier invoice date
 */
void ACC_PurchaseOrder::createMembers() {
    addMember(this, "orderno", "-", 0, RB2::MemberInteger);
    addMember(this, "requisitionno", "-", "", RB2::MemberChar125);
    addMember(this, "receiptat_id", "-", "0", RB2::MemberChar40);
    addMember(this, "orderdate", "-", "2000-01-01 00:00:00", RB2::MemberDateTime);
    addMember(this, "contact_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "contactphone", "-", "", RB2::MemberChar20);
    addMember(this, "contactemail", "-", "", RB2::MemberChar125);
    addMember(this, "supplierref", "-", "", RB2::MemberChar125);
    addMember(this, "receiptdate", "-", "2000-01-01 00:00:00", RB2::MemberDateTime);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);
    addMember(this, "shipvia_id", "-", "0", RB2::MemberChar40);
    addMember(this, "orderstatus", "-", 0, RB2::MemberInteger);
    addMember(this, "quotation", "-", 0, RB2::MemberInteger); // 0=no, 1=yes

    addMember(this, "deladd1", "-", "", RB2::MemberChar125);
    addMember(this, "deladd2", "-", "", RB2::MemberChar125);
    addMember(this, "deladd3", "-", "", RB2::MemberChar125);
    addMember(this, "deladd4", "-", "", RB2::MemberChar125);
    addMember(this, "deladd5", "-", "", RB2::MemberChar125);
    addMember(this, "deladd6", "-", "", RB2::MemberChar125);

    addMember(this, "ordertitle", "-", "", RB2::MemberChar125);
    addMember(this, "comments", "-", "", RB2::MemberChar2500);

    addMember(this, "suppinvoiceno", "-", "", RB2::MemberChar125);
    addMember(this, "suppinvoicedate", "-", "", RB2::MemberDateTime);



//    addMember(this, "buyername", "-", "", RB2::MemberChar125);
//    addMember(this, "ordertype", "-", "", RB2::MemberChar10);
//    addMember(this, "freightcost", "-", 0.0, RB2::MemberDouble);
//    addMember(this, "fromstkloc_id", "-", "0", RB2::MemberChar40);
//    addMember(this, "quotedate", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
//    addMember(this, "confirmeddate", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
//    addMember(this, "printedpackingslip", "-", 0, RB2::MemberInteger);
//    addMember(this, "datepackingslipprinted", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
}

