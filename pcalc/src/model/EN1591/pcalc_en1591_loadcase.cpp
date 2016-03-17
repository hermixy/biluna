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
    addMember("loadcaseno", "-", 0, RB2::MemberInteger);
    // pressure temperature
    addMember("p", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("t0", "Celsius", 0.0, RB2::MemberDouble);
    addMember("tb", "Celsius", 0.0, RB2::MemberDouble);
    addMember("tg", "Celsius", 0.0, RB2::MemberDouble);
    // flange 1
    addMember("tf1", "Celsius", 0.0, RB2::MemberDouble);
    addMember("tl1", "Celsius", 0.0, RB2::MemberDouble);
    addMember("tw1", "Celsius", 0.0, RB2::MemberDouble);
    // flange 2
    addMember("tf2", "Celsius", 0.0, RB2::MemberDouble);
    addMember("tl2", "Celsius", 0.0, RB2::MemberDouble);
    addMember("tw2", "Celsius", 0.0, RB2::MemberDouble);
    // force
    addMember("fx", "N", 0.0, RB2::MemberDouble);
    addMember("fy", "N", 0.0, RB2::MemberDouble);
    addMember("fz", "N", 0.0, RB2::MemberDouble);
    addMember("mx", "Nmm", 0.0, RB2::MemberDouble);
    addMember("my", "Nmm", 0.0, RB2::MemberDouble);
    addMember("mz", "Nmm", 0.0, RB2::MemberDouble);
    // stress expansion elasticity
    addMember("fb", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("relaxb", "perunage", 1.0, RB2::MemberDouble);
    addMember("fn", "N/mm2", 0.0, RB2::MemberDouble);

    // flange 1
    addMember("ff1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("fl1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("fs1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("fw1", "N/mm2", 0.0, RB2::MemberDouble);
    // flange 2
    addMember("ff2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("fl2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("fs2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("fw2", "N/mm2", 0.0, RB2::MemberDouble);

    addMember("alphab", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember("alphag", "1/Kelvin", 0.0, RB2::MemberDouble);
    // flange 1
    addMember("alphaf1", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember("alphal1", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember("alphaw1", "1/Kelvin", 0.0, RB2::MemberDouble);
    // flange 2
    addMember("alphaf2", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember("alphal2", "1/Kelvin", 0.0, RB2::MemberDouble);
    addMember("alphaw2", "1/Kelvin", 0.0, RB2::MemberDouble);

    addMember("eb", "N/mm2", 0.0, RB2::MemberDouble);
    // flange 1
    addMember("ef1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("el1", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("ew1", "N/mm2", 0.0, RB2::MemberDouble);
    // flange 2
    addMember("ef2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("el2", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("ew2", "N/mm2", 0.0, RB2::MemberDouble);
}

