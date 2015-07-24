/*****************************************************************
 * $Id: acc_stockmove.cpp 2193 2014-10-27 21:37:20Z rutger $
 * Created: Mar 25, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockmove.h"

ACC_StockMove::ACC_StockMove (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_StockMove::ACC_StockMove(ACC_StockMove* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_StockMove::~ACC_StockMove() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li order_idx sales or internal work order
 * \li serialno serial number from order or number if no order exists
 * \li type_idx move type based on work process, each move has a counter part
 * to return/reverse the move, such as to warehouse, used for manufacturing
 * to assemble in new product (stock), return to supplier, etc,
 * \li location_idx physical address of the warehouse(s)
 * \li building_idx warehouse building and/or room in warehouse
 * \li row_idx row in warehouse (room)
 * \li box_idx box in row of warehouse
 * \li qty quantity of the move
 * \li movedata date of the move
 * \li employee_idx employee responsible for the move
 * \li status_id status: for comments (to be returned),
 * to next phase (can be returned), final (can not be returned)
 * \li comment remarks, narrative, supporting text
 *
 * what, from/to where, how much, when, who
 */
void ACC_StockMove::createMembers() {
    addMember(this, "order_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "serialno", "-", "", RB2::MemberChar40);
    addMember(this, "type_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "location_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "building_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "row_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "box_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "qty", "-", 0.0, RB2::MemberDouble);
    addMember(this, "movedate", "-", "1971-01-01 00:00:00", RB2::MemberDateTime);
    addMember(this, "employee_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "status_id", "-", 0, RB2::MemberInteger);
    addMember(this, "comment", "-", "", RB2::MemberChar125);
}


/* *
 * Create members:
 * - serialno serial number from stock master, manual entered or automatic if serialized
 * - type type of movement
 * - transno transaction number
 * - location_id location ID
 * - transdate transaction date
 * - debtor_id debtor ID
 * - branch_id customer branch ID
 * - price price
 * - prd period
 * - salesorder_id sales order ID
 * - qty quantity
 * - discountpercent discount percentage
 * - standardcost standard cost
 * - show_on_inv_crds 1 is show on invoice cards
 * - newqoh new quantity on hand
 * - hidemovt 1 is hide movement
 * - narrative narrative for stock movement
 * - taxcat_id tax category id for easy retrieval for example during setting of tax rates
 * - soline_id salesorder line ID, to reference stockmove to salesorder line
 * /
void ACC_StockMove::createMembers() {
    addMember(this, "serialno", "-", 0, RB2::MemberChar40);
    addMember(this, "type", "-", 0, RB2::MemberInteger);          // 3
    addMember(this, "transno", "-", "", RB2::MemberChar40);       // rel_id
    addMember(this, "location_id", "-", "", RB2::MemberChar40);       // rel_id
    addMember(this, "transdate", "-", "0000-00-00 00:00:00", RB2::MemberDateTime);
    addMember(this, "debtor_id", "-", "", RB2::MemberChar40);      // rel_id
    addMember(this, "branch_id", "-", "", RB2::MemberChar40);    // rel_id
    addMember(this, "price", "-", 0.0, RB2::MemberDouble);
    addMember(this, "prd", "-", 0, RB2::MemberInteger);           // period
    addMember(this, "salesorder_id", "-", "", RB2::MemberChar40);     // rel_id salesorderId
    addMember(this, "qty", "-", 0.0, RB2::MemberDouble);          // quantity (dispatched)
    addMember(this, "discountpercent", "-", 0.0, RB2::MemberDouble);
    addMember(this, "standardcost", "-", 0.0, RB2::MemberDouble);
    addMember(this, "show_on_inv_crds", "-", 1, RB2::MemberInteger);
    addMember(this, "newqoh", "-", 0.0, RB2::MemberDouble);       // new quantity on hand
    addMember(this, "hidemovt", "-", 0, RB2::MemberInteger);
    addMember(this, "narrative", "-", "", RB2::MemberChar255);
    // Added:
    // Tax category id for easy retrieval for example during setting of tax rates
    addMember(this, "taxcat_id", "-", "0", RB2::MemberChar40);     // rel_id
    // Salesorder line ID, to reference stockmove to salesorder line
    addMember(this, "soline_id", "-", "0", RB2::MemberChar40);     // rel_id salesorderlineId
}
*/
