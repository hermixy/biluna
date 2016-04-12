/*****************************************************************
 * $Id: std_en13555pqrdeltaegc.h 0001 2016-04-124T16:53:06 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_en13555qsmaxalphag.h"

STD_EN13555QsmaxAlphaG::STD_EN13555QsmaxAlphaG(
                    const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_EN13555QsmaxAlphaG::STD_EN13555QsmaxAlphaG(STD_EN13555QsmaxAlphaG* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

STD_EN13555QsmaxAlphaG::~STD_EN13555QsmaxAlphaG() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temp temperature [Celsius]
 * \li qsmax maximum allowable gasket pressure [N/mm2]
 * \li alphag thermal expansion of the gasket [1/K]
 */
void STD_EN13555QsmaxAlphaG::createMembers() {
    addMember("temp", "Celsius", 0.0, RB2::MemberDouble);
    addMember("qsmax", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("alphag", "1/K", 0.0, RB2::MemberDouble);
}
