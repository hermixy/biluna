/*****************************************************************
 * $Id: acc_salestaxposting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 19, 2012 13:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salestaxposting.h"


ACC_SalesTaxPosting::ACC_SalesTaxPosting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_SalesTaxPosting::ACC_SalesTaxPosting(ACC_SalesTaxPosting* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_SalesTaxPosting::~ACC_SalesTaxPosting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - taxauth_id tax authority ID
 * - taxcat_id tax category ID
 * - salestaxaccount_idx sales tax account ID and description
 */
void ACC_SalesTaxPosting::createMembers() {
    addMember("taxauth_id", "-", "0", RB2::MemberChar40);
    addMember("taxcat_id", "-", "0", RB2::MemberChar40);
    addMember("salestaxacct_idx", "-", "0", RB2::MemberChar165);
}
