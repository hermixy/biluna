/*****************************************************************
 * $Id: acc_unitofmeasure.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 16, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_unitofmeasure.h"

ACC_UnitOfMeasure::ACC_UnitOfMeasure (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_UnitOfMeasure::ACC_UnitOfMeasure(ACC_UnitOfMeasure* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_UnitOfMeasure::~ACC_UnitOfMeasure() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - unitname name of the unit of measurement
 */
void ACC_UnitOfMeasure::createMembers() {
    addMember(this, "unitname", "-", "", RB2::MemberChar125);      // 3
}
