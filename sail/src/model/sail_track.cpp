/*****************************************************************
 * $Id: sail_track.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_track.h"

SAIL_Track::SAIL_Track (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SAIL_Track::SAIL_Track(SAIL_Track* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SAIL_Track::~SAIL_Track() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
  Create members:
  \li mname, name of the track
  \li comment, additional comment with the track
  \li from_idx, From coordinate
  \li to_idx, To coordinate
  \li weightfactor, a track usually gets a fixed distance
*/
void SAIL_Track::createMembers() {
    addMember(this, "mname", "-", "<NEW>", RB2::MemberChar125);
    addMember(this, "comment", "-", "", RB2::MemberChar255);
    addMember(this, "from_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "to_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "weightfactor", "-", 0.0, RB2::MemberDouble);
}
