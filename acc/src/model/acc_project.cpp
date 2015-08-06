/*****************************************************************
 * $Id: acc_project.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Dec 18, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_project.h"


ACC_Project::ACC_Project (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Project::ACC_Project(ACC_Project* project) : RB_ObjectContainer(project) {
    createMembers();
    *this = *project;
}


ACC_Project::~ACC_Project() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - coyname company name
 * - gstno goods and services tax reference number
 * - companynumber company number in certain countries, Chamber of Commerce number
 * - regoffice1 - 6 registered office address line 1 - 6
 * - telephone telephone
 * - fax fax number
 * - email email address
 * - currencydefault_id default currency ID
 * - pytdiscountact_idx payment discount account ID and description
 * - payrollact_idx payroll payment account ID and description
 * - grnact_idx goods received note account ID and description
 * - exchangediffact_idx sales exchange differences account ID and description
 * - purchasesexchangediffact_idx purchase exchange differences account ID and description
 * - retainedearnings_idx (net earnings - dividends paid =) retained earnings ID and description
 * - freightact_idx freight cost account ID and description
 * - bankname bank name
 * - bankiban bank IBAN nubmer
 * - bankbicswift bank BIC or Swift number
 * - taxprovice_id dispatch tax province ID
 * - taxgroup_id delivery tax location ID
 * - logo company logo image
 */
void ACC_Project::createMembers() {
    addMember("coyname", "-", "<Your Company>", RB2::MemberChar125);
    //! Company goods sales tax reference number, VAT number
    addMember("gstno", "-", "", RB2::MemberChar20);
    addMember("companynumber", "-", "0", RB2::MemberChar20);
    addMember("regoffice1", "-", "", RB2::MemberChar125);
    addMember("regoffice2", "-", "", RB2::MemberChar125);
    addMember("regoffice3", "-", "", RB2::MemberChar125);
    addMember("regoffice4", "-", "", RB2::MemberChar125);
    addMember("regoffice5", "-", "", RB2::MemberChar125);
    addMember("regoffice6", "-", "", RB2::MemberChar125);
    addMember("telephone", "-", "", RB2::MemberChar40);
    addMember("fax", "-", "", RB2::MemberChar20);
    addMember("email", "-", "", RB2::MemberChar125);
    addMember("currencydefault_id", "-", "0", RB2::MemberChar40);
    addMember("pytdiscountact_idx", "-", "0", RB2::MemberChar165);
    addMember("payrollact_idx", "-", "0", RB2::MemberChar165);
    addMember("grnact_idx", "-", "0", RB2::MemberChar165);
    addMember("exchangediffact_idx", "-", "0", RB2::MemberChar165);
    addMember("purchasesexchangediffact_idx", "-", "0", RB2::MemberChar165);
    addMember("retainedearnings_idx", "-", "0", RB2::MemberChar165);
    addMember("freightact_idx", "-", "0", RB2::MemberChar165);
    // ACC addition
    addMember("bankname", "-", "", RB2::MemberChar125);
    addMember("bankiban", "-", "", RB2::MemberChar125);
    addMember("bankbicswift", "-", "", RB2::MemberChar125);

    addMember("taxprovince_id", "-", "0", RB2::MemberChar40); // default dispatch tax province
    addMember("taxgroup_id", "-", "0", RB2::MemberChar40); // default delivery tax location
    // Logo's, barcodes
    addMember("logo", "-", "<Your Company>", RB2::MemberChar255);
    addMember("fiscalmonthstart", "-", 1, RB2::MemberInteger);
    addMember("fiscaldaystart", "-", 1, RB2::MemberInteger);
}

