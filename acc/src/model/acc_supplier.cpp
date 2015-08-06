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
    addMember("suppliercode", "-", "", RB2::MemberChar40);        // 3
    addMember("suppname", "-", "", RB2::MemberChar125);
    addMember("address1", "-", "", RB2::MemberChar125);
    addMember("address2", "-", "", RB2::MemberChar125);
    addMember("address3", "-", "", RB2::MemberChar125);
    addMember("address4", "-", "", RB2::MemberChar125);
    addMember("address5", "-", "", RB2::MemberChar125);
    addMember("address6", "-", "", RB2::MemberChar40);
    addMember("lat", "-", 0.000000, RB2::MemberDouble);
    addMember("lng", "-", 0.000000, RB2::MemberDouble);
    addMember("currency_id", "-", "0", RB2::MemberChar40);
    addMember("suppliersince", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember("paymentterm_id", "-", "0", RB2::MemberChar40);
    addMember("lastpaid", "-", 0.0, RB2::MemberDouble);
    addMember("lastpaiddate", "-", "2000-01-01T00:00:00", RB2::MemberDateTime);
    addMember("bankname", "-", "", RB2::MemberChar125); // Bank name
    addMember("bankaddress", "-", "", RB2::MemberChar255);
    addMember("bankcode", "-", "", RB2::MemberChar40); // swift/ABA/routing
    addMember("bankaccountnumber", "-", "", RB2::MemberChar40); // IBAN/BIC
    addMember("remittance", "-", 0, RB2::MemberInteger);
    addMember("taxgroup_id", "-", "0", RB2::MemberChar40);
    addMember("factorcompany_id", "-", "0", RB2::MemberChar40);
    addMember("taxref", "-", "", RB2::MemberChar20);
    addMember("phn", "-", "", RB2::MemberChar125);
    addMember("port", "-", "", RB2::MemberChar255);

    addMember("phoneno", "-", "", RB2::MemberChar20);
    addMember("faxno", "-", "", RB2::MemberChar20);
    addMember("contactname", "-", "", RB2::MemberChar125);
    addMember("email", "-", "", RB2::MemberChar125);

    // accounting
    addMember("lastusedacct_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember("lastusedcostcenter_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember("area_id", "-", "0", RB2::MemberChar40);  // rel_id

    // supplier relation management 0=potential supplier, 1=existing supplier
    addMember("srmtype_id", "-", 0, RB2::MemberInteger);  // int_id
    addMember("srm_parent", "-", "0", RB2::MemberChar40); // is SRM Account (SRM_Project.id)
}
