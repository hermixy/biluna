/*****************************************************************
 * $Id: pcalc_en13555qminl.cpp 0001 2016-01-24T16:31:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_en13555qminl.h"

PCALC_EN13555QminL::PCALC_EN13555QminL(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

PCALC_EN13555QminL::PCALC_EN13555QminL(PCALC_EN13555QminL* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

PCALC_EN13555QminL::~PCALC_EN13555QminL() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void PCALC_EN13555QminL::createMembers() {
    addMember("testpress", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("leakrate", "-", 0.0, RB2::MemberDouble);
    addMember("qminl", "N/mm2", 0.0, RB2::MemberDouble);
}
