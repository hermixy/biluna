/*****************************************************************
 * $Id: sail_route.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 29, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_route.h"

SAIL_Route::SAIL_Route (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SAIL_Route::SAIL_Route(SAIL_Route* obj) : RB_ObjectContainer(obj) {
    createMembers();
    *this = *obj;
}


SAIL_Route::~SAIL_Route() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
  Create members:
  \li mname, name of the route
  \li comment, history of purpose of the route
*/
void SAIL_Route::createMembers() {
    addMember(this, "mname", "-", 0, RB2::MemberChar125);
    addMember(this, "comment", "-", "", RB2::MemberChar255);
}
