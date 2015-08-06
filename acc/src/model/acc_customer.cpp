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
    addMember("customerno", "-", "", RB2::MemberChar40);      // 3
    addMember("mname", "-", "", RB2::MemberChar125);         // 4
    addMember("brpostaddress1", "-", "", RB2::MemberChar125);
    addMember("brpostaddress2", "-", "", RB2::MemberChar125);
    addMember("brpostaddress3", "-", "", RB2::MemberChar125);
    addMember("brpostaddress4", "-", "", RB2::MemberChar125);
    addMember("brpostaddress5", "-", "", RB2::MemberChar125);
    addMember("brpostaddress6", "-", "", RB2::MemberChar40);
    addMember("brstreetaddress1", "-", "", RB2::MemberChar125);
    addMember("brstreetaddress2", "-", "", RB2::MemberChar125);
    addMember("brstreetaddress3", "-", "", RB2::MemberChar125);
    addMember("brstreetaddress4", "-", "", RB2::MemberChar125);
    addMember("brstreetaddress5", "-", "", RB2::MemberChar125);
    addMember("brstreetaddress6", "-", "", RB2::MemberChar40);
    addMember("currency_id", "-", "0", RB2::MemberChar40);     // rel_id
    addMember("salestype_id", "-", "0", RB2::MemberChar40);    // rel_id
    addMember("clientsince", "-", "", RB2::MemberChar20);
    addMember("holdreason_id", "-", "0", RB2::MemberChar40);   // rel_id
    addMember("paymentterm_id", "-", "0", RB2::MemberChar40); // rel_id
    addMember("discount", "-", 0.0, RB2::MemberDouble);
    addMember("pymtdiscount", "-", 0.0, RB2::MemberDouble);
    addMember("lastpaid", "-", 0.0, RB2::MemberDouble);
    addMember("lastpaiddate", "-", "", RB2::MemberChar20);
    addMember("bankname", "-", "", RB2::MemberChar125); // Bank name
    addMember("bankaddress", "-", "", RB2::MemberChar255);
    addMember("bankcode", "-", "", RB2::MemberChar40); // swift/ABA/routing
    addMember("bankaccountnumber", "-", "", RB2::MemberChar40); // IBAN/BIC
    addMember("creditlimit", "-", 0.0, RB2::MemberDouble);
    addMember("invaddrbranch", "-", 0, RB2::MemberInteger); // int_id TODO yes/no int
    addMember("discountcode", "-", "", RB2::MemberChar10);
    addMember("ediinvoices", "-", 0, RB2::MemberInteger);
    addMember("ediorders", "-", 0, RB2::MemberInteger);
    addMember("edireference", "-", "", RB2::MemberChar20);
    addMember("editransport", "-", "", RB2::MemberChar10);
    addMember("ediaddress", "-", "", RB2::MemberChar125);
    addMember("ediserveruser", "-", "", RB2::MemberChar20);
    addMember("ediserverpwd", "-", "", RB2::MemberChar20);
    addMember("taxref", "-", "", RB2::MemberChar20);
    addMember("customerpoline", "-", 0, RB2::MemberInteger);// int_id TODO yes/no int
    addMember("type_id", "-", "0", RB2::MemberChar40);       // rel_id customer type
    addMember("custparent_idx", "-", "0", RB2::MemberChar165);// rel_id administration parent company

    // custbranch
    addMember("lat", "-", 0.0, RB2::MemberDouble); // latitude (GeoCoordinate)
    addMember("lng", "-", 0.0, RB2::MemberDouble); // longitude (GeoCoordinate)
    addMember("estdeliverydays", "-", 0, RB2::MemberInteger);
    addMember("area_id", "-", "0", RB2::MemberChar40);
    addMember("salesman_id", "-", "0", RB2::MemberChar40);
    addMember("fwddate", "-", "", RB2::MemberDateTime);
    addMember("phoneno", "-", "", RB2::MemberChar20);
    addMember("faxno", "-", "", RB2::MemberChar20);
    addMember("contactname", "-", "", RB2::MemberChar40);
    addMember("email", "-", "", RB2::MemberChar125);
    addMember("defaultlocation_id", "-", "0", RB2::MemberChar40);  // dispatch location
    addMember("taxgroup_id", "-", "0", RB2::MemberChar40);      // rel_id
    addMember("defaultshipvia_id", "-", "0", RB2::MemberChar40);   // rel_id
    addMember("deliverblind", "-", 0, RB2::MemberInteger);
    addMember("disabletrans", "-", 0, RB2::MemberInteger);
    addMember("specialinstructions", "-", "", RB2::MemberChar125);
    addMember("custbranchcode", "-", "", RB2::MemberChar40);

    // accounting
    addMember("lastusedacct_id", "-", "0", RB2::MemberChar40);  // rel_id
    addMember("lastusedcostcenter_id", "-", "0", RB2::MemberChar40);  // rel_id

    // customer relation management 0=potential customer, 1=existing customer
    addMember("crmtype_id", "-", 0, RB2::MemberInteger);  // int_id
    addMember("crm_parent", "-", "0", RB2::MemberChar40); // is CRM Account (CRM_Project.id)
}

