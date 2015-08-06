/*****************************************************************
 * $Id: sail_symbol.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_symbol.h"

SAIL_Symbol::SAIL_Symbol (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


SAIL_Symbol::SAIL_Symbol(SAIL_Symbol* obj) : RB_ObjectAtomic(obj) {
    createMembers();
    *this = *obj;
}


SAIL_Symbol::~SAIL_Symbol() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
  Create members:
  \li description, description of the symbol
  \li svg, the SVG data of the symbol image
 */
void SAIL_Symbol::createMembers() {
    addMember("mname", "-", "<New>", RB2::MemberChar125);
    addMember("svg", "-", "", RB2::MemberString);
    addMember("icon", "-", "", RB2::MemberByteArray);
}
