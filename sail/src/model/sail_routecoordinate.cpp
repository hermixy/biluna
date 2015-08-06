/*****************************************************************
 * $Id: sail_routecoordinate.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 29, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_routecoordinate.h"

SAIL_RouteCoordinate::SAIL_RouteCoordinate (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SAIL_RouteCoordinate::SAIL_RouteCoordinate(SAIL_RouteCoordinate* obj)
                    : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SAIL_RouteCoordinate::~SAIL_RouteCoordinate() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
  Create members:
  \li seqno, sequence number of the coordinate. Use increments of 10
  to be able to insert others when required
  \li comment, comment with the coordinate
  \li coordinate_idx, coordinate ID and name
*/
void SAIL_RouteCoordinate::createMembers() {
    addMember("seqno", "-", 0, RB2::MemberInteger);
    addMember("coordinate_idx", "-", "0", RB2::MemberChar165);
    addMember("comment", "-", "", RB2::MemberChar255);
}
