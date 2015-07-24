/*****************************************************************
 * $Id: pcalc_component.cpp 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591_assembly.h"


PCALC_EN1591_Assembly::PCALC_EN1591_Assembly(const RB_String& id,
                                           RB_ObjectBase *p,
                                           const RB_String& n,
                                           RB_ObjectFactory* f)
                                        : RB_ObjectContainer(id, p, n, f) {
	 createMembers();
}

PCALC_EN1591_Assembly::PCALC_EN1591_Assembly(PCALC_EN1591_Assembly* component)
            : RB_ObjectContainer(component) {
	createMembers();
	*this = *component;
}

PCALC_EN1591_Assembly::~PCALC_EN1591_Assembly() {
    // nothing
}

/**
 * Create members:
 * \li number or code for this flange assembly
 * \li description assembly description
 * \li revision revision of the data
 * \li date date of calculation
 * \li nameengineer name of engineer
 * \li nr (mNR) number of reassemblies
 */
void PCALC_EN1591_Assembly::createMembers() {
    addMember(this, "number", "-", "<NEW>", RB2::MemberChar40);
    addMember(this, "description", "-", "", RB2::MemberChar125);
    addMember(this, "revision", "-", 0, RB2::MemberInteger);
    addMember(this, "date", "-", "", RB2::MemberDateTime);
    addMember(this, "nameengineer", "-", "", RB2::MemberChar125);
 }
