/*****************************************************************
 * $Id: acc_gltrans.cpp 2204 2015-01-16 14:51:02Z rutger $
 * Created: Apr 1, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_gltrans.h"

ACC_GlTrans::ACC_GlTrans (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_GlTrans::ACC_GlTrans(ACC_GlTrans* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_GlTrans::~ACC_GlTrans() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - type GL transaction type
 *     TransBankCash = 13
 *     TransCreditor = 20
 *     TransDebtor = 10
 *     TransMemo = 5
 *     TransPurchOrder = 40 (not invoice, based on order)
 *     TransSalesOrder = 30 (not invoice, based on order)
 * - transdocno transaction document number
 * - chequeno cheque number (used?)
 * - transdate transaction date
 * - periodno period number e.g. 201206 is June 2012
 * - description description with the line item
 * - chartmaster_idx GL account ID and description
 * - accountcontrol account control number
 * - amount GL amount
 * - posted is 1 if posted in chartdetails, done by operation ACC_CreateGlTrialBalance
 * - jobref_id job or project reference ID (used?)
 * - tag_id tag ID (used?)
 * - transdoc_id transaction document ID
 * - itemtrans_id bank(cash)/creditor/debtor/memo transaction item
 * - transallocn_idx transaction allocation ID, and invoice or bank statement number
 * - costcenter_idx cost center ID and description
 */
void ACC_GlTrans::createMembers() {
    addMember(this, "type", "-", 0, RB2::MemberInteger);
    addMember(this, "transdocno", "-", 0, RB2::MemberInteger);
    addMember(this, "chequeno", "-", 0, RB2::MemberInteger);
    addMember(this, "transdate", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember(this, "periodno", "-", 0, RB2::MemberInteger);
    addMember(this, "description", "-", "", RB2::MemberChar255);
    addMember(this, "chartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "accountcontrol", "-", 0, RB2::MemberInteger);
    addMember(this, "amount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "posted", "-", 0, RB2::MemberInteger);
    addMember(this, "jobref_id", "-", "0", RB2::MemberChar40);
    addMember(this, "tag_id", "-", "0", RB2::MemberChar40);
    addMember(this, "transdoc_id", "-", "0", RB2::MemberChar40);
    addMember(this, "itemtrans_id", "-", "0", RB2::MemberChar40);
    addMember(this, "transallocn_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "costcenter_idx", "-", "0", RB2::MemberChar165);
}
