/*****************************************************************
 * $Id: pcalc_en13555qsminl.cpp 0001 2016-01-24T16:47:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_en13555qsminl.h"

PCALC_EN13555QsminL::PCALC_EN13555QsminL(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

PCALC_EN13555QsminL::PCALC_EN13555QsminL(PCALC_EN13555QsminL* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

PCALC_EN13555QsminL::~PCALC_EN13555QsminL() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void PCALC_EN13555QsminL::createMembers() {
    addMember("testpress", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("leakrate", "-", 0.0, RB2::MemberDouble);
    addMember("qa", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("qsminl", "N/mm2", 0.0, RB2::MemberDouble);
}
