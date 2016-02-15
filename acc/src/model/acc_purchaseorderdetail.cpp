/*****************************************************************
 * $Id: acc_purchaseorderdetail.cpp 2027 2013-11-08 19:05:44Z rutger $
 * Created: Sept 22, 2011 10:04:25 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchaseorderdetail.h"

ACC_PurchaseOrderDetail::ACC_PurchaseOrderDetail (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_PurchaseOrderDetail::ACC_PurchaseOrderDetail(ACC_PurchaseOrderDetail* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_PurchaseOrderDetail::~ACC_PurchaseOrderDetail() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - stk_idx stock item ID and description
 * - serialno serial number from stock master, manual entered or automatic if serialized
 * - stkdescription stock description
 * - stkuom stock unit of measurement
 * - unitprice unit price
 * - quantity quantity ordered
 * - estimate 1 if this is an estimate only
 * - qtyinvoiced quantity already invoiced by the supplier
 * - qtyreceived quantity as received by the warehouse, reset to 0.0 after
 *    invoice is processed
 * - discountpercent discount percent from supplier
 * - actualreceiptdate actual receipt date
 * - completed 1 means order is completed
 * - narrative narrative with this order item
 * - poline purchase order line description
 * - itemdue item to be received as per order
 * - taxcat_id tax category ID for this item, for easy retrieval, for example
 *    during the creation of stockmoves
 * - costcenter_idx cost center ID and description
 */
void ACC_PurchaseOrderDetail::createMembers() {
    addMember("stk_idx", "-", "0", RB2::MemberChar165);
    addMember("serialno", "-", "", RB2::MemberChar40);
    addMember("stkdescription", "-", "", RB2::MemberChar125);
    addMember("stkuom", "-", "", RB2::MemberChar20);
    addMember("unitprice", "-", 0.0, RB2::MemberDouble);
    addMember("quantity", "-", 0.0, RB2::MemberDouble);
    addMember("estimate", "-", 0, RB2::MemberInteger);
    addMember("qtyinvoiced", "-", 0.0, RB2::MemberDouble);
    // Quantity received when creating the invoice, reset to 0.0 after invoice is processed
    addMember("qtyreceived", "-", 0.0, RB2::MemberDouble);
    addMember("discountpercent", "-", 0.0, RB2::MemberDouble);
    addMember("actualreceiptdate", "-", "2000-01-01 00:00:00", RB2::MemberDateTime);
    addMember("completed", "-", 0, RB2::MemberInteger);
    addMember("narrative", "-", "", RB2::MemberChar255);
    addMember("poline", "-", "", RB2::MemberChar125);
    addMember("itemdue", "-", "1900-01-01", RB2::MemberDateTime);
    // Tax category id for easy retrieval for example during creation of stockmoves
    addMember("taxcat_id", "-", "0", RB2::MemberChar40);
    addMember("costcenter_idx", "-", "0", RB2::MemberChar165);
}
