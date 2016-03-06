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
 * \li gaskettype_idx id plus gasket type
 * \li formtype_id id of gasket form type
 * \li manufacturetype type of manufacturing
 * \li insertfilltype_id id of gasket filler
 * \li nr number of reassemblies
 * \li leakagerate leakage rate L
 * \li dg0 first contact diameter of oval gasket types
 * \li dgin inside diameter
 * \li dgout outside diameter
 * \li dg1en13555 EN13555 test gasket inside diameter
 * \li dg2en13555 EN13555 test gasket outside diameter
 * \li egt theoretical thickness
 * \li k stiffness of test rig
 * \li phig angle of triangular cross section gasket
 * \li r2 radius of oval or triangular cross section gasket
 */
void PCALC_EN1591_Gasket::createMembers() {
    addMember("gaskettype_idx", "-", "0", RB2::MemberChar165);
    addMember("formtype_id", "-", 0, RB2::MemberInteger);
    addMember("manufacturetype", "-", 0, RB2::MemberChar125);
    addMember("insertfilltype_id", "-", 0, RB2::MemberInteger);
    addMember("nr", "-", 0, RB2::MemberInteger);
    addMember("leakagerate", "-", 0, RB2::MemberInteger);
    addMember("dg0", "mm", 0.0, RB2::MemberDouble);
    addMember("dgin", "mm", 0.0, RB2::MemberDouble);
    addMember("dgout", "mm", 0.0, RB2::MemberDouble);
    addMember("dg1en13555", "mm", 0.0, RB2::MemberDouble);
    addMember("dg2en13555", "mm", 0.0, RB2::MemberDouble);
    addMember("egt", "mm", 0.0, RB2::MemberDouble);
    addMember("k", "-", 0.0, RB2::MemberDouble);
    addMember("phig", "-", 0.0, RB2::MemberDouble);
    addMember("r2", "mm", 0.0, RB2::MemberDouble);
}
