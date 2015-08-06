/*****************************************************************
 * $Id: acc_cogsglposting.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 02, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_cogsglposting.h"


ACC_CogsGlPosting::ACC_CogsGlPosting (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_CogsGlPosting::ACC_CogsGlPosting(ACC_CogsGlPosting* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_CogsGlPosting::~ACC_CogsGlPosting() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - area_id ID of the sales area
 * - stkcat_idx stock category ID and description
 * - glcode_idx GL account ID and description
 */
void ACC_CogsGlPosting::createMembers() {
    addMember("area_id", "-", "0", RB2::MemberChar40);
    addMember("stkcat_idx", "-", "0", RB2::MemberChar165);
    addMember("glcode_idx", "-", "0", RB2::MemberChar165);
}
