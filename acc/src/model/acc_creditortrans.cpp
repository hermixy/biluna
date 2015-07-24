/*****************************************************************
 * $Id: acc_creditortrans.cpp 2099 2014-02-17 19:48:13Z rutger $
 * Created: Feb 27, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_creditortrans.h"


ACC_CreditorTrans::ACC_CreditorTrans (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CreditorTrans::ACC_CreditorTrans(ACC_CreditorTrans* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CreditorTrans::~ACC_CreditorTrans() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - transdoc_id ID of transaction document
 * - description description of this Creditor line item
 * - charmaster_idx GL account ID and description
 * - accountcontrol control number of this account
 * - amount amount of this line item
 * - taxhighchartmaster_idx high tax GL account ID and description
 * - taxhighamt amount of high tax
 * - taxlowchartmaster_idx low tax GL account ID and description
 * - taxlowamt amount of low tax
 * - taxotherchartmaster_idx other tax GL account ID and description
 * - taxotheramt amount of other tax
 * - taxauth_id ID of tax authority
 * - hold 1 if this line item is on hold
 * - orderdetail_id reference ID to purchase order detail ID (ACC_PurchaseOrderDetail.id)
 * - stk_idx stock item ID and description
 * - narrative additional description which showe on the order
 * - unitprice price of an item unit
 * - qtyreceived quantity received
 * - stkuom stock item unit of measurement
 * - discountpercent discount percentage
 * - costcenter_idx cost center ID and description
 */
void ACC_CreditorTrans::createMembers() {
    addMember(this, "transdoc_id", "-", "0", RB2::MemberChar40);
    addMember(this, "description", "-", "", RB2::MemberChar255);
    addMember(this, "chartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "accountcontrol", "-", 0, RB2::MemberInteger);
    addMember(this, "amount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "taxhighchartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "taxhighamt", "-", 0.0, RB2::MemberDouble);
    addMember(this, "taxlowchartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "taxlowamt", "-", 0.0, RB2::MemberDouble);
    addMember(this, "taxotherchartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "taxotheramt", "-", 0.0, RB2::MemberDouble);
    addMember(this, "taxauth_id", "-", "0", RB2::MemberChar40);
    addMember(this, "hold", "-", 0, RB2::MemberInteger);
    // Link to order detail, ACC_PurchaseOrderDetail
    addMember(this, "orderdetail_id", "-", "0", RB2::MemberChar40);
    addMember(this, "stk_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "narrative", "-", "", RB2::MemberChar255);
    addMember(this, "unitprice", "-", 0.0, RB2::MemberDouble);
    addMember(this, "qtyreceived", "-", 0.0, RB2::MemberDouble);
    addMember(this, "stkuom", "-", "", RB2::MemberChar20);
    addMember(this, "discountpercent", "-", 0.0, RB2::MemberDouble);
    addMember(this, "costcenter_idx", "-", "0", RB2::MemberChar165);
}
