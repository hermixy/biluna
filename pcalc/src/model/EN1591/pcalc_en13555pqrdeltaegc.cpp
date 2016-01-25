/*****************************************************************
 * $Id: pcalc_en13555pqrdeltaegc.cpp 0001 2016-01-24T16:53:06 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_en13555pqrdeltaegc.h"

PCALC_En13555PqrDeltaeGC::PCALC_En13555PqrDeltaeGC(
                    const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

PCALC_En13555PqrDeltaeGC::PCALC_En13555PqrDeltaeGC(PCALC_En13555PqrDeltaeGC* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
	*this = *project;
}

PCALC_En13555PqrDeltaeGC::~PCALC_En13555PqrDeltaeGC() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void PCALC_En13555PqrDeltaeGC::createMembers() {
    addMember("c", "kN/mm", 0.0, RB2::MemberDouble);
    addMember("qg", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("temp", "Celsius", 0.0, RB2::MemberDouble);
    addMember("pqr", "-", 0.0, RB2::MemberDouble);
    addMember("deltaegc", "mm", 0.0, RB2::MemberDouble);
}
