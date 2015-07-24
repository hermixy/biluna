/*****************************************************************
 * $Id: acc_bankimport.cpp 2153 2014-07-25 16:38:28Z rutger $
 * Created: Oct 15, 2012 19:41:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_bankimport.h"

ACC_BankImport::ACC_BankImport (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


ACC_BankImport::ACC_BankImport(ACC_BankImport* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


ACC_BankImport::~ACC_BankImport() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members
 * - importmessage, system import message
 * - statbankname, statement bank name
 * - statactnumber, statement account number                (Rekening nummer)
 * - statstartamount, statement start amount
 * - statendamount, statement end amount
 * - stattimestamp, statement time stamp                    (Datum afschrift)
 * - statnumber, statement number                           (Afschrift nummer)
 * - transaccount, transaction account name                 (Naam/Omschrijving, tegenrekening)
 * - transactnumber, transaction account number             (Tegenrekening nummer)
 * - transamount, transaction amount                        (Bedrag EUR)
 * - transdebitcredit, transaction debit or credit, D or C  (Af/Bij)
 * - transdescription, transaction description              (Beschrijving)
 * - transdescription, transaction second description       (Mededelingen)
 * - transentrytimestamp, transaction entry time stamp      (Datum transactie)
 * - transvaluetimestamp, transaction value time stamp
 * - transcode, transaction code                            (Transactie Code/Mutatie Soort)
 */
void ACC_BankImport::createMembers() {
    addMember(this, "importmessage", "-", "", RB2::MemberChar125);
    addMember(this, "statbankname", "-", "", RB2::MemberChar125);
    addMember(this, "statactnumber", "-", "", RB2::MemberChar40);
    addMember(this, "statstartamount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "statendamount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "stattimestamp", "-", "", RB2::MemberChar20);
    addMember(this, "statnumber", "-", "0", RB2::MemberChar40);
    addMember(this, "transaccount", "-", "", RB2::MemberChar40);
    addMember(this, "transactnumber", "-", "", RB2::MemberChar40);
    addMember(this, "transamount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "transdebitcredit", "-", "", RB2::MemberChar10);
    addMember(this, "transdescription", "-", "", RB2::MemberChar125);
    addMember(this, "transseconddescription", "-", "", RB2::MemberChar125);
    addMember(this, "transentrytimestamp", "-", "", RB2::MemberChar20);
    addMember(this, "transvaluetimestamp", "-", "", RB2::MemberChar20);
    addMember(this, "transcode", "-", "", RB2::MemberChar125);
}
