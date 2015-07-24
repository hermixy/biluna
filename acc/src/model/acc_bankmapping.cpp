/*****************************************************************
 * $Id: acc_bankmapping.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 01, 2013 10:04:25 AM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_bankmapping.h"

ACC_BankMapping::ACC_BankMapping (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_BankMapping::ACC_BankMapping(ACC_BankMapping* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_BankMapping::~ACC_BankMapping() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - bankaccount current bank account number
 * - dateformat format of the date field, example: yyyyMMdd (20130721 for July 21, 2013)
 * - creditname bank credit identification if numbers are not negative,
 *   for example: 'Af' (Dutch). Empty if not applicable.
 * - separator_id value separator i.e.: comma, semi-colon or tab
 * - firstrowtitle_id indicates whether first row is a title row only
 * - stripquotes_id indicator whether single or double quotes need to be stripped from value
 * - decimal_id indicator wheter point or comma is used as decimal separator
 * - colnobankaccount column number of bank account
 * - colnotransactiondate column number of transaction date
 * - colnocounterbankaccount column number of counter bank account
 * - colnocounteraccountholder column number of counter account holder name
 * - colnodebitcredit column number of debit/credit indicator
 * - colnoamount column number of amount
 * - colnodescription column number of description
 * - colnoseconddescription column number of second description
 * - colnotransactioncode column number of transaction code
 * - colnostatnumber, bank statement sequence number
 */
void ACC_BankMapping::createMembers() {
    addMember(this, "bankaccount", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "dateformat", "-", "yyyy-MM-dd", RB2::MemberChar20);
    addMember(this, "creditname", "-", "", RB2::MemberChar20);
    addMember(this, "separator_id", "-", 0, RB2::MemberInteger);
    addMember(this, "firstrowtitle_id", "-", 0, RB2::MemberInteger);
    addMember(this, "stripquotes_id", "-", 0, RB2::MemberInteger);
    addMember(this, "decimal_id", "-", 0, RB2::MemberInteger);
    addMember(this, "colnobankaccount", "-", 0, RB2::MemberInteger);
    addMember(this, "colnotransactiondate", "-", 0, RB2::MemberInteger);
    addMember(this, "colnocounterbankaccount", "-", 0, RB2::MemberInteger);
    addMember(this, "colnocounteraccountholder", "-", 0, RB2::MemberInteger);
    addMember(this, "colnodebitcredit", "-", 0, RB2::MemberInteger);
    addMember(this, "colnoamount", "-", 0, RB2::MemberInteger);
    addMember(this, "colnodescription", "-", 0, RB2::MemberInteger);
    addMember(this, "colnoseconddescription", "-", 0, RB2::MemberInteger);
    addMember(this, "colnotransactioncode", "-", 0, RB2::MemberInteger);
    addMember(this, "colnostatnumber", "-", 0, RB2::MemberInteger);
}
