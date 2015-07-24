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
    addMember(this, "nr", "-", 0, RB2::MemberInteger);
    addMember(this, "nb", "-", 0, RB2::MemberInteger);

    // flange 1
    addMember(this, "typeflange1_id", "-", 0, RB2::MemberInteger);
    addMember(this, "d01", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d31", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d41", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "dx1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "ef1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "eft1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "ep1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "erf1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "ex1", "mm", 0.0, RB2::MemberDouble);
    // blind only
    addMember(this, "d91", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "e01", "mm", 0.0, RB2::MemberDouble);
    // integral only
    addMember(this, "d11", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d21", "mm", 0.0, RB2::MemberDouble);
    // integral loose only
    addMember(this, "e11", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "e21", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "lh1", "mm", 0.0, RB2::MemberDouble);
    // loose only
    addMember(this, "b01", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d61", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d81", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "el1", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "materialflange1_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "materialloosering1_idx", "-", "0", RB2::MemberChar165);

    // flange 2
    addMember(this, "flange2equal", "-", 0, RB2::MemberInteger);

    addMember(this, "typeflange2_id", "-", 0, RB2::MemberInteger);
    addMember(this, "d02", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d32", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d42", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "dx2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "ef2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "eft2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "ep2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "erf2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "ex2", "mm", 0.0, RB2::MemberDouble);
    // blind only
    addMember(this, "d92", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "e02", "mm", 0.0, RB2::MemberDouble);
    // integral only
    addMember(this, "d12", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d22", "mm", 0.0, RB2::MemberDouble);
    // integral loose only
    addMember(this, "e12", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "e22", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "lh2", "mm", 0.0, RB2::MemberDouble);
    // loose only
    addMember(this, "b02", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d62", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "d82", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "el2", "mm", 0.0, RB2::MemberDouble);
    addMember(this, "materialflange2_idx", "-", "0", RB2::MemberChar165);
    addMember(this, "materialloosering2_idx", "-", "0", RB2::MemberChar165);
}
