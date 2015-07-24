/*****************************************************************
 * $Id: sail_coordinate.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_coordinate.h"

SAIL_Coordinate::SAIL_Coordinate (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SAIL_Coordinate::SAIL_Coordinate(SAIL_Coordinate* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SAIL_Coordinate::~SAIL_Coordinate() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
  Create members:
  \li mname, name of the coordinate such as buoy name
  \li comment, explanation or name of the coordinate
  \li symbol_idx, symbol ID such as buoy symbol
  \li latitude, always negative/positive (min. is -90.0, max. 90.0)
  in south/north direction. Approximately 69 miles
  \li longitude, always negative/positive (min. is -180.0, max. 180.0)
  in west/east direction
*/
void SAIL_Coordinate::createMembers() {
    addMember(this, "mname", "-", "<New>", RB2::MemberChar125);
    addMember(this, "comment", "-", "", RB2::MemberChar255);
    addMember(this, "symbol_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "latitude", "degree", 0.0, RB2::MemberDouble);
    addMember(this, "longitude", "degree", 0.0, RB2::MemberDouble);
}
