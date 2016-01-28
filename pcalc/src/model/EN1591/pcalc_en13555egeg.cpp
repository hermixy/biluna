/*****************************************************************
 * $Id: pcalc_en13555egeg.cpp 0001 2016-01-24T16:56:54 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_en13555egeg.h"

PCALC_En13555EGeG::PCALC_En13555EGeG(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

PCALC_En13555EGeG::PCALC_En13555EGeG(PCALC_En13555EGeG* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

PCALC_En13555EGeG::~PCALC_En13555EGeG() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void PCALC_En13555EGeG::createMembers() {
    addMember("qg", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("temp", "Celsius", 0.0, RB2::MemberDouble);
    addMember("capitaleg", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("eg", "mm", 0.0, RB2::MemberDouble);
}
