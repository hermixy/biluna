/*****************************************************************
 * $Id: acc_salesorderdetail.cpp 2027 2013-11-08 19:05:44Z rutger $
 * Created: Jan 23, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesorderdetail.h"

ACC_SalesOrderDetail::ACC_SalesOrderDetail (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SalesOrderDetail::ACC_SalesOrderDetail(ACC_SalesOrderDetail* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SalesOrderDetail::~ACC_SalesOrderDetail() {
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
 * - qtyinvoiced quantity already invoiced
 * - qtydispatched quantity as dispatched by the warehouse, reset to 0.0 after
 *    invoice is processed
 * - estimate 1 if this is an estimate only
 * - discountpercent discount percent from supplier
 * - actualdispatchdate actual dispatch date
 * - completed 1 means order is completed
 * - narrative narrative with this order item
 * - itemdue item to be received as per order
 * - poline purchase order line description
 * - taxcat_id tax category ID for this item, for easy retrieval, for example
 *    during the creation of stockmoves
 * - costcenter_idx cost center ID and description
 */
void ACC_SalesOrderDetail::createMembers() {
    addMember(this, "stk_idx", "-", "0", RB2::MemberChar165);         // rel_id
    addMember(this, "serialno", "-", "", RB2::MemberChar40);
    addMember(this, "stkdescription", "-", "", RB2::MemberChar125);
    addMember(this, "stkuom", "-", "", RB2::MemberChar20);
    addMember(this, "unitprice", "-", 0.0, RB2::MemberDouble);
    addMember(this, "quantity", "-", 0.0, RB2::MemberDouble);       // order quantity
    addMember(this, "qtyinvoiced", "-", 0.0, RB2::MemberDouble);    // already invoiced
    // Quantity dispatched when creating the invoice, reset to 0.0 after invoice is processed
    addMember(this, "qtydispatched", "-", 0.0, RB2::MemberDouble);
    addMember(this, "estimate", "-", 0, RB2::MemberInteger);
    addMember(this, "discountpercent", "-", 0.0, RB2::MemberDouble);
    addMember(this, "actualdispatchdate", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
    addMember(this, "completed", "-", 0, RB2::MemberInteger);
    addMember(this, "narrative", "-", "", RB2::MemberChar255);
    addMember(this, "itemdue", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
    addMember(this, "poline", "-", "", RB2::MemberChar125);
    // Tax category id for easy retrieval for example during creation of stockmoves
    addMember(this, "taxcat_id", "-", "0", RB2::MemberChar40);         // rel_id
    addMember(this, "costcenter_idx", "-", "0", RB2::MemberChar165);
}
