/*****************************************************************
 * $Id: acc_debtortrans.cpp 2099 2014-02-17 19:48:13Z rutger $
 * Created: Feb 27, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_debtortrans.h"


ACC_DebtorTrans::ACC_DebtorTrans (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_DebtorTrans::ACC_DebtorTrans(ACC_DebtorTrans* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_DebtorTrans::~ACC_DebtorTrans() {
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
void ACC_DebtorTrans::createMembers() {
    addMember("transdoc_id", "-", "0", RB2::MemberChar40);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("chartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember("accountcontrol", "-", 0, RB2::MemberInteger);
    addMember("amount", "-", 0.0, RB2::MemberDouble);
    addMember("taxhighchartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember("taxhighamt", "-", 0.0, RB2::MemberDouble);
    addMember("taxlowchartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember("taxlowamt", "-", 0.0, RB2::MemberDouble);
    addMember("taxotherchartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember("taxotheramt", "-", 0.0, RB2::MemberDouble);
    addMember("taxauth_id", "-", "0", RB2::MemberChar40);
    addMember("hold", "-", 0, RB2::MemberInteger);
    // Link to order detail, ACC_SalesOrderDetail
    addMember("orderdetail_id", "-", "0", RB2::MemberChar40);
    addMember("stk_idx", "-", "0", RB2::MemberChar165);
    addMember("narrative", "-", "", RB2::MemberChar255);
    addMember("unitprice", "-", 0.0, RB2::MemberDouble);
    addMember("qtydispatched", "-", 0.0, RB2::MemberDouble);
    addMember("stkuom", "-", "", RB2::MemberChar20);
    addMember("discountpercent", "-", 0.0, RB2::MemberDouble);
    addMember("costcenter_idx", "-", "0", RB2::MemberChar165);

}
