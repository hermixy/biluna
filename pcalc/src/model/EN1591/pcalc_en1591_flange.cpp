/*****************************************************************
 * $Id: pcalc_component.cpp 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591_flange.h"


PCALC_EN1591_Flange::PCALC_EN1591_Flange(const RB_String& id,
                                           RB_ObjectBase *p,
                                           const RB_String& n,
                                           RB_ObjectFactory* f)
                                        : RB_ObjectAtomic(id, p, n, f) {
	 createMembers();
}

PCALC_EN1591_Flange::PCALC_EN1591_Flange(PCALC_EN1591_Flange* component)
            : RB_ObjectAtomic(component) {
	createMembers();
	*this = *component;
}

PCALC_EN1591_Flange::~PCALC_EN1591_Flange() {
    // nothing
}

/**
 * Create members:
 * \li -
 */
void PCALC_EN1591_Flange::createMembers() {
    addMember("nr", "-", 0, RB2::MemberInteger);
    addMember("nb", "-", 0, RB2::MemberInteger);
    addMember("leakagerate", "-", "", RB2::MemberInteger);
    addMember("f_bspecified", "-", "N", RB2::MemberDouble);

    // flange 1
    addMember("typeflange1_id", "-", 0, RB2::MemberInteger);
    addMember("d01", "mm", 0.0, RB2::MemberDouble);
    addMember("d31", "mm", 0.0, RB2::MemberDouble);
    addMember("d41", "mm", 0.0, RB2::MemberDouble);
    addMember("dx1", "mm", 0.0, RB2::MemberDouble);
    addMember("ef1", "mm", 0.0, RB2::MemberDouble);
    addMember("eft1", "mm", 0.0, RB2::MemberDouble);
    addMember("ep1", "mm", 0.0, RB2::MemberDouble);
    addMember("erf1", "mm", 0.0, RB2::MemberDouble);
    addMember("ex1", "mm", 0.0, RB2::MemberDouble);
    // blind only
    addMember("d91", "mm", 0.0, RB2::MemberDouble);
    addMember("e01", "mm", 0.0, RB2::MemberDouble);
    // integral only
    addMember("d11", "mm", 0.0, RB2::MemberDouble);
    addMember("d21", "mm", 0.0, RB2::MemberDouble);
    // integral loose only
    addMember("e11", "mm", 0.0, RB2::MemberDouble);
    addMember("e21", "mm", 0.0, RB2::MemberDouble);
    addMember("lh1", "mm", 0.0, RB2::MemberDouble);
    // loose only
    addMember("b01", "mm", 0.0, RB2::MemberDouble);
    addMember("d61", "mm", 0.0, RB2::MemberDouble);
    addMember("d81", "mm", 0.0, RB2::MemberDouble);
    addMember("el1", "mm", 0.0, RB2::MemberDouble);
    addMember("materialflange1_idx", "-", "0", RB2::MemberChar165);
    addMember("materialloosering1_idx", "-", "0", RB2::MemberChar165);

    // flange 2
    addMember("flange2equal", "-", 0, RB2::MemberInteger);

    addMember("typeflange2_id", "-", 0, RB2::MemberInteger);
    addMember("d02", "mm", 0.0, RB2::MemberDouble);
    addMember("d32", "mm", 0.0, RB2::MemberDouble);
    addMember("d42", "mm", 0.0, RB2::MemberDouble);
    addMember("dx2", "mm", 0.0, RB2::MemberDouble);
    addMember("ef2", "mm", 0.0, RB2::MemberDouble);
    addMember("eft2", "mm", 0.0, RB2::MemberDouble);
    addMember("ep2", "mm", 0.0, RB2::MemberDouble);
    addMember("erf2", "mm", 0.0, RB2::MemberDouble);
    addMember("ex2", "mm", 0.0, RB2::MemberDouble);
    // blind only
    addMember("d92", "mm", 0.0, RB2::MemberDouble);
    addMember("e02", "mm", 0.0, RB2::MemberDouble);
    // integral only
    addMember("d12", "mm", 0.0, RB2::MemberDouble);
    addMember("d22", "mm", 0.0, RB2::MemberDouble);
    // integral loose only
    addMember("e12", "mm", 0.0, RB2::MemberDouble);
    addMember("e22", "mm", 0.0, RB2::MemberDouble);
    addMember("lh2", "mm", 0.0, RB2::MemberDouble);
    // loose only
    addMember("b02", "mm", 0.0, RB2::MemberDouble);
    addMember("d62", "mm", 0.0, RB2::MemberDouble);
    addMember("d82", "mm", 0.0, RB2::MemberDouble);
    addMember("el2", "mm", 0.0, RB2::MemberDouble);
    addMember("materialflange2_idx", "-", "0", RB2::MemberChar165);
    addMember("materialloosering2_idx", "-", "0", RB2::MemberChar165);
}
