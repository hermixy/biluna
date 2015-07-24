/*****************************************************************
 * $Id: pcalc_component.cpp 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591_loadcase.h"


PCALC_EN1591_LoadCase::PCALC_EN1591_LoadCase(const RB_String& id,
                                           RB_ObjectBase* p,
                                           const RB_String& n,
                                           RB_ObjectFactory* f)
                                        : RB_ObjectAtomic(id, p, n, f) {
	 createMembers();
}

PCALC_EN1591_LoadCase::PCALC_EN1591_LoadCase(PCALC_EN1591_LoadCase* loadCase)
            : RB_ObjectAtomic(loadCase) {
	createMembers();
    *this = *loadCase;
}

PCALC_EN1591_LoadCase::~PCALC_EN1591_LoadCase() {
    // nothing
}

/**
 * Create members:
 * \li -
 */
void PCALC_EN1591_LoadCase::createMembers() {
    addMember(this, "loadcaseno", "-", 0, RB2::MemberInteger);
    // pressure temperature
    addMember(this, "p", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "t0", "Celsius", 0.0, RB2::MemberDouble);
    addMember(this, "tb", "Celsius", 0.0, RB2::MemberDouble);
    addMember(this, "tg", "Celsius", 0.0, RB2::MemberDouble);
    // flange 1
    addMember(this, "tf1", "Celsius", 0.0, RB2::MemberDouble);
    addMember(this, "tl1", "Celsius", 0.0, RB2::MemberDouble);
    addMember(this, "tw1", "Celsius", 0.0, RB2::MemberDouble);
    // flange 2
    addMember(this, "tf2", "Celsius", 0.0, RB2::MemberDouble);
    addMember(this, "tl2", "Celsius", 0.0, RB2::MemberDouble);
    addMember(this, "tw2", "Celsius", 0.0, RB2::MemberDouble);
    // force
    addMember(this, "fx", "N", 0.0, RB2::MemberDouble);
    addMember(this, "fy", "N", 0.0, RB2::MemberDouble);
    addMember(this, "fz", "N", 0.0, RB2::MemberDouble);
    addMember(this, "mx", "N", 0.0, RB2::MemberDouble);
    addMember(this, "my", "N", 0.0, RB2::MemberDouble);
    addMember(this, "mz", "N", 0.0, RB2::MemberDouble);
    // stress expansion elasticity
    addMember(this, "fb", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fn", "N/mm2", 0.0, RB2::MemberDouble);

    // flange 1
    addMember(this, "ff1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fl1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fs1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fw1", "N/mm2", 0.0, RB2::MemberDouble);
    // flange 2
    addMember(this, "ff2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fl2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fs2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "fw2", "N/mm2", 0.0, RB2::MemberDouble);

    addMember(this, "alphab", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember(this, "alphag", "1/Kelvin", 0.0, RB2::MemberDouble);
    // flange 1
    addMember(this, "alphaf1", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember(this, "alphal1", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember(this, "alphaw1", "1/Kelvin", 0.0, RB2::MemberDouble);
    // flange 2
    addMember(this, "alphaf2", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember(this, "alphal2", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember(this, "alphaw2", "1/Kelvin", 0.0, RB2::MemberDouble);

    addMember(this, "eb", "N/mm2", 0.0, RB2::MemberDouble);
    // flange 1
    addMember(this, "ef1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "el1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "ew1", "N/mm2", 0.0, RB2::MemberDouble);
    // flange 2
    addMember(this, "ef2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "el2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember(this, "ew2", "N/mm2", 0.0, RB2::MemberDouble);
}

