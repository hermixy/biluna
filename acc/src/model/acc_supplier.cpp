/*****************************************************************
 * $Id: acc_supplier.cpp 2181 2014-09-30 15:34:36Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_supplier.h"

ACC_Supplier::ACC_Supplier (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Supplier::ACC_Supplier(ACC_Supplier* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_Supplier::~ACC_Supplier() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - suppliercode supplier code usually capitals
 * - suppname name of the supplier
 * - address1 - 6 addresses
 * - lat latitude
 * - lng longitude
 * - currency_id currency ID
 * - suppliersince date of first order
 * - paymentterm_id payment terms ID
 * - lastpaid last paid amount
 * - bankact bank account
 * - bankref bank reference
 * - bankpartics bank particulars
 * - remittance remittance advice, letter indicating that invoice is paid
 * - taxgroup_id tax location ID
 * - factorcompany_id factor company ID
 * - taxref supplier tax reference number
 * - phn (used?)
 * - port (used?)
 * - phoneno phone number
 * - faxno fax number
 * - contactname contact name
 * - email email address
 * - lastusedacct_id last used GL account ID
 * - area_id purchase area ID
 * - srmtype_id SRM type ID 0=potential 1=existing
 * - srm_paprent SRM Account (SRM_Project.id)
 */
void ACC_Supplier::createMembers() {
    addMember(this, "suppliercode", "-", "", RB2::MemberChar40);        // 3
    addMember(this, "suppname", "-", "", RB2::MemberChar125);
    addMember(this, "address1", "-", "", RB2::MemberChar125);
    addMember(this, "address2", "-", "", RB2::MemberChar125);
    addMember(this, "address3", "-", "", RB2::MemberChar125);
    addMember(this, "address4", "-", "", RB2::MemberChar125);
    addMember(this, "address5", "-", "", RB2::MemberChar125);
    addMember(this, "address6", "-", "", RB2::MemberChar40);
    addMember(this, "lat", "-", 0.000000, RB2::MemberDouble);
    addMember(this, "lng", "-", 0.000000, RB2::MemberDouble);
    addMember(this, "currency_id", "-", "0", RB2::MemberChar40);
    addMember(this, "suppliersince", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember(this, "paymentterm_id", "-", "0", RB2::MemberChar40);
    addMember(this, "lastpaid", "-", 0.0, RB2::MemberDouble);
    addMember(this, "lastpaiddate", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember(this, "bankname", "-", "", RB2::MemberChar125); // Bank name
    addMember(this, "bankaddress", "-", "", RB2::MemberChar255);
    addMember(this, "bankcode", "-", "", RB2::MemberChar40); // swift/ABA/routing
    addMember(this, "bankaccountnumber", "-", "", RB2::MemberChar40); // IBAN/BIC
    addMember(this, "remittance", "-", 0, RB2::MemberInteger);
    addMember(this, "taxgroup_id", "-", "0", RB2::MemberChar40);
    addMember(this, "factorcompany_id", "-", "0", RB2::MemberChar40);
    addMember(this, "taxref", "-", "", RB2::MemberChar20);
    addMember(this, "phn", "-", "", RB2::MemberChar125);
    addMember(this, "port", "-", "", RB2::MemberChar255);

    addMember(this, "phoneno", "-", "", RB2::MemberChar20);
    addMember(this, "faxno", "-", "", RB2::MemberChar20);
    addMember(this, "contactname", "-", "", RB2::MemberChar125);
    addMember(this, "email", "-", "", RB2::MemberChar125);

    // accounting
    addMember(this, "lastusedacct_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember(this, "lastusedcostcenter_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember(this, "area_id", "-", "0", RB2::MemberChar40);  // rel_id

    // supplier relation management 0=potential supplier, 1=existing supplier
    addMember(this, "srmtype_id", "-", 0, RB2::MemberInteger);  // int_id
    addMember(this, "srm_parent", "-", "0", RB2::MemberChar40); // is SRM Account (SRM_Project.id)
}
