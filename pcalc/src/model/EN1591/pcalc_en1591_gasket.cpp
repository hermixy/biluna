/*****************************************************************
 * $Id: pcalc_component.cpp 1826 2012-12-08 00:31:33Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Biluna. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591_gasket.h"


PCALC_EN1591_Gasket::PCALC_EN1591_Gasket(const RB_String& id,
                                           RB_ObjectBase *p,
                                           const RB_String& n,
                                           RB_ObjectFactory* f)
                                        : RB_ObjectAtomic(id, p, n, f) {
	 createMembers();
}

PCALC_EN1591_Gasket::PCALC_EN1591_Gasket(PCALC_EN1591_Gasket* component)
            : RB_ObjectAtomic(component) {
	createMembers();
	*this = *component;
}

PCALC_EN1591_Gasket::~PCALC_EN1591_Gasket() {
    // nothing
}

/**
 * Create members:
 * \li -
 */
void PCALC_EN1591_Gasket::createMembers() {
    addMember("materialcode_ix", "-", "0", RB2::MemberChar165);
    addMember("formtype_id", "-", 0, RB2::MemberInteger);
    addMember("manufacturetype", "-", 0, RB2::MemberChar125);
    addMember("insertfilltype_id", "-", 0, RB2::MemberInteger);
    addMember("dg0", "mm", 0.0, RB2::MemberDouble);
    addMember("dg1", "mm", 0.0, RB2::MemberDouble);
    addMember("dg2", "mm", 0.0, RB2::MemberDouble);
    addMember("dg1en13555", "mm", 0.0, RB2::MemberDouble);
    addMember("dg2en13555", "mm", 0.0, RB2::MemberDouble);
    addMember("egt", "mm", 0.0, RB2::MemberDouble);
    addMember("k", "-", 0.0, RB2::MemberDouble);
    addMember("phig", "-", 0.0, RB2::MemberDouble);
    addMember("r2", "mm", 0.0, RB2::MemberDouble);
}
