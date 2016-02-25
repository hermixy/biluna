/*****************************************************************
 * $Id: pcalc_en13555pqrdeltaegc.cpp 0001 2016-01-24T16:53:06 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_en13555pqrdeltaegc.h"

PCALC_EN13555PqrDeltaeGC::PCALC_EN13555PqrDeltaeGC(
                    const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

PCALC_EN13555PqrDeltaeGC::PCALC_EN13555PqrDeltaeGC(PCALC_EN13555PqrDeltaeGC* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

PCALC_EN13555PqrDeltaeGC::~PCALC_EN13555PqrDeltaeGC() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li c test rig stifness [kN/mm], 'k' is used in calculations
 * \li qa gasket strees at assembly [N/mm2],
 * the highest one at temperature is Qsmax
 * \li temp temperature [Celsius]
 * \li pqr Pqr [-]
 * \li deltaegc delta eGC [mm]
 */
void PCALC_EN13555PqrDeltaeGC::createMembers() {
    addMember("c", "kN/mm", 0.0, RB2::MemberDouble);
    addMember("temp", "Celsius", 0.0, RB2::MemberDouble);
    addMember("qa", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("pqr", "-", 0.0, RB2::MemberDouble);
    addMember("deltaegc", "mm", 0.0, RB2::MemberDouble);
}
