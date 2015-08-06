/*****************************************************************
 * $Id: acc_banktrans.cpp 2163 2014-09-04 08:03:35Z rutger $
 * Created: Sep 2, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_banktrans.h"


ACC_BankTrans::ACC_BankTrans (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_BankTrans::ACC_BankTrans(ACC_BankTrans* obj)
                    : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_BankTrans::~ACC_BankTrans() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - transdoc_id ID of transaction document
 * - description description of this bank line item
 * - chartmaster_idx GL account ID and description
 * - accountcontrol account control \todo purpose unclear
 * - amount this bank document item amount
 * - type type of transaction cash or type
 * - transno transaction number of the transaction document
 * - bankaccountnumber counter bank account number, only with bank import
 * - amountcleared amount cleared (allocated)
 * - exrate exchange rate (by user?)
 * - functionexrate functional exchange rate (by system?)
 * - transdate transaction date
 * - banktranstype bank transaction type \todo not used
 * - currency_id currency ID
 * - transallocn_idx allocation to document ID and description
 */
void ACC_BankTrans::createMembers() {
    addMember("transdoc_id", "-", "0", RB2::MemberChar40);
    addMember("description", "-", "", RB2::MemberChar255);
    addMember("chartmaster_idx", "-", "0", RB2::MemberChar165);
    addMember("accountcontrol", "-", 0, RB2::MemberInteger);
    addMember("amount", "-", 0.0, RB2::MemberDouble);
    addMember("type", "-", 0, RB2::MemberInteger);
    addMember("transno", "-", 0, RB2::MemberInteger);
    addMember("bankaccountnumber", "-", "", RB2::MemberChar40);
    addMember("amountcleared", "-", 0.0, RB2::MemberDouble);
    addMember("exrate", "-", 1.0, RB2::MemberDouble);
    addMember("functionalexrate", "-", 1.0, RB2::MemberDouble);
    addMember("transdate", "-", "0000-00-00T00:00:00", RB2::MemberDateTime);
    addMember("banktranstype", "-", "", RB2::MemberChar40);
    addMember("currency_id", "-", "0", RB2::MemberChar40);
    addMember("transallocn_idx", "-", "0", RB2::MemberChar165);
}
