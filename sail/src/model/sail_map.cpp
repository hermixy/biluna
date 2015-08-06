/*****************************************************************
 * $Id: sail_map.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_map.h"

SAIL_Map::SAIL_Map (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SAIL_Map::SAIL_Map(SAIL_Map* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SAIL_Map::~SAIL_Map() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
  Create members:
  \li mname name of the map
  \li version version of the map
  \li svg SVG data of the map
 */
void SAIL_Map::createMembers() {
    addMember("mname", "-", "", RB2::MemberChar125);
    addMember("version", "-", "", RB2::MemberChar125);
    addMember("svg", "-", "", RB2::MemberString);
}
