/*****************************************************************
 * $Id: acc_taxauthrate.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 22, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxauthrate.h"

ACC_TaxAuthRate::ACC_TaxAuthRate (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_TaxAuthRate::ACC_TaxAuthRate(ACC_TaxAuthRate* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_TaxAuthRate::~ACC_TaxAuthRate() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - dispatchprovince_id dispatch province ID
 * - taxcat_id tax category ID
 * - taxrate tax rate as percentage
 */
void ACC_TaxAuthRate::createMembers() {
    addMember("dispatchtaxprovince_id", "-", "0", RB2::MemberChar40);
    addMember("taxcat_id", "-", "0", RB2::MemberChar40);
    addMember("taxrate", "percent", 0.0, RB2::MemberDouble);
}
