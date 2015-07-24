/*****************************************************************
 * $Id: acc_customer.cpp 2181 2014-09-30 15:34:36Z rutger $
 * Created: Dec 23, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_customer.h"


ACC_Customer::ACC_Customer (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Customer::ACC_Customer(ACC_Customer* branch) : RB_ObjectContainer(branch) {
    createMembers();
    *this = *branch;
}


ACC_Customer::~ACC_Customer() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - customerno customer code usually capitals
 * - mname name of the customer
 * - brpostaddress1 - 6 postal addresses
 * - brstreetaddress1 - 6 street addresses
 * - currency_id currency ID
 * - salestype_id sales type ID
 * - clientsince date of first order
 * - holdreaseon_id hold reason ID
 * - paymentterm_id payment terms ID
 * - discount discount in percent
 * - pymtdiscount early payment discount
 * - lastpaid last paid amount
 * - lastpaiddate date of last payment
 * - creditlimit maximum credit limit of open invoices
 * - invaddrbranch use invoice address of branch or parent company
 * - ediinvoices Electronic Data Interchange (EDI) protocol invoice
 * - ediorders EDI order
 * - edireference EDI reference
 * - editransport EDI transport
 * - ediaddress EDI address
 * - ediserveruser EDI server username
 * - ediserverpwd EDI server password
 * - taxref customer tax reference number
 * - customerpoline require customer purchase order line to be printed on shipping documents
 * - type_id customer type ID
 * - custparent_idx customer parent company ID and description
 * - lat latitude
 * - lng longitude
 * - estdeliverydays estimated days for delivery
 * - area_id sales area ID
 * - salesman_id salesman ID
 * - fwddate forward date after (day in month)
 * - phoneno phone number
 * - faxno fax number
 * - contactname contact name
 * - email email address
 * - defaultlocation_id dispatch location ID
 * - taxgroup_id tax location ID
 * - defaultshipvia_id shipping method DHL TNT or other
 * - deliverblind deliver blindly
 * - disabletrans disable transactions with this customer
 * - specialinstructions special instructions
 * - custbranchcode customer branch code (used?)
 * - lastusedacct_id last used GL account ID
 * - crmtype_id CRM type ID 0=potential 1=existing
 * - crm_paprent CRM Account (CRM_Project.id)
 */
void ACC_Customer::createMembers() {
    addMember(this, "customerno", "-", "", RB2::MemberChar40);      // 3
    addMember(this, "mname", "-", "", RB2::MemberChar125);         // 4
    addMember(this, "brpostaddress1", "-", "", RB2::MemberChar125);
    addMember(this, "brpostaddress2", "-", "", RB2::MemberChar125);
    addMember(this, "brpostaddress3", "-", "", RB2::MemberChar125);
    addMember(this, "brpostaddress4", "-", "", RB2::MemberChar125);
    addMember(this, "brpostaddress5", "-", "", RB2::MemberChar125);
    addMember(this, "brpostaddress6", "-", "", RB2::MemberChar40);
    addMember(this, "brstreetaddress1", "-", "", RB2::MemberChar125);
    addMember(this, "brstreetaddress2", "-", "", RB2::MemberChar125);
    addMember(this, "brstreetaddress3", "-", "", RB2::MemberChar125);
    addMember(this, "brstreetaddress4", "-", "", RB2::MemberChar125);
    addMember(this, "brstreetaddress5", "-", "", RB2::MemberChar125);
    addMember(this, "brstreetaddress6", "-", "", RB2::MemberChar40);
    addMember(this, "currency_id", "-", "0", RB2::MemberChar40);     // rel_id
    addMember(this, "salestype_id", "-", "0", RB2::MemberChar40);    // rel_id
    addMember(this, "clientsince", "-", "", RB2::MemberChar20);
    addMember(this, "holdreason_id", "-", "0", RB2::MemberChar40);   // rel_id
    addMember(this, "paymentterm_id", "-", "0", RB2::MemberChar40); // rel_id
    addMember(this, "discount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "pymtdiscount", "-", 0.0, RB2::MemberDouble);
    addMember(this, "lastpaid", "-", 0.0, RB2::MemberDouble);
    addMember(this, "lastpaiddate", "-", "", RB2::MemberChar20);
    addMember(this, "bankname", "-", "", RB2::MemberChar125); // Bank name
    addMember(this, "bankaddress", "-", "", RB2::MemberChar255);
    addMember(this, "bankcode", "-", "", RB2::MemberChar40); // swift/ABA/routing
    addMember(this, "bankaccountnumber", "-", "", RB2::MemberChar40); // IBAN/BIC
    addMember(this, "creditlimit", "-", 0.0, RB2::MemberDouble);
    addMember(this, "invaddrbranch", "-", 0, RB2::MemberInteger); // int_id TODO yes/no int
    addMember(this, "discountcode", "-", "", RB2::MemberChar10);
    addMember(this, "ediinvoices", "-", 0, RB2::MemberInteger);
    addMember(this, "ediorders", "-", 0, RB2::MemberInteger);
    addMember(this, "edireference", "-", "", RB2::MemberChar20);
    addMember(this, "editransport", "-", "", RB2::MemberChar10);
    addMember(this, "ediaddress", "-", "", RB2::MemberChar125);
    addMember(this, "ediserveruser", "-", "", RB2::MemberChar20);
    addMember(this, "ediserverpwd", "-", "", RB2::MemberChar20);
    addMember(this, "taxref", "-", "", RB2::MemberChar20);
    addMember(this, "customerpoline", "-", 0, RB2::MemberInteger);// int_id TODO yes/no int
    addMember(this, "type_id", "-", "0", RB2::MemberChar40);       // rel_id customer type
    addMember(this, "custparent_idx", "-", "0", RB2::MemberChar165);// rel_id administration parent company

    // custbranch
    addMember(this, "lat", "-", 0.0, RB2::MemberDouble); // latitude (GeoCoordinate)
    addMember(this, "lng", "-", 0.0, RB2::MemberDouble); // longitude (GeoCoordinate)
    addMember(this, "estdeliverydays", "-", 0, RB2::MemberInteger);
    addMember(this, "area_id", "-", "0", RB2::MemberChar40);
    addMember(this, "salesman_id", "-", "0", RB2::MemberChar40);
    addMember(this, "fwddate", "-", "", RB2::MemberDateTime);
    addMember(this, "phoneno", "-", "", RB2::MemberChar20);
    addMember(this, "faxno", "-", "", RB2::MemberChar20);
    addMember(this, "contactname", "-", "", RB2::MemberChar40);
    addMember(this, "email", "-", "", RB2::MemberChar125);
    addMember(this, "defaultlocation_id", "-", "0", RB2::MemberChar40);  // dispatch location
    addMember(this, "taxgroup_id", "-", "0", RB2::MemberChar40);      // rel_id
    addMember(this, "defaultshipvia_id", "-", "0", RB2::MemberChar40);   // rel_id
    addMember(this, "deliverblind", "-", 0, RB2::MemberInteger);
    addMember(this, "disabletrans", "-", 0, RB2::MemberInteger);
    addMember(this, "specialinstructions", "-", "", RB2::MemberChar125);
    addMember(this, "custbranchcode", "-", "", RB2::MemberChar40);

    // accounting
    addMember(this, "lastusedacct_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember(this, "lastusedcostcenter_id", "-", "0", RB2::MemberChar40);  // rel_id

    // customer relation management 0=potential customer, 1=existing customer
    addMember(this, "crmtype_id", "-", 0, RB2::MemberInteger);  // int_id
    addMember(this, "crm_parent", "-", "0", RB2::MemberChar40); // is CRM Account (CRM_Project.id)
}

