/*****************************************************************
 * $Id: acc_area.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jan 20, 2010 10:04:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_area.h"

ACC_Area::ACC_Area (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


ACC_Area::ACC_Area(ACC_Area* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


ACC_Area::~ACC_Area() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * - areacode code for the sales or purchase area, usually in capitals
 * - areadescription description for this area
 */
void ACC_Area::createMembers() {
    addMember(this, "areacode", "-", "", RB2::MemberChar40);
    addMember(this, "areadescription", "-", "", RB2::MemberChar125);
}
