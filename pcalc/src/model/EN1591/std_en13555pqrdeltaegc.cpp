/*****************************************************************
 * $Id: std_en13555pqrdeltaegc.cpp 0001 2016-01-24T16:53:06 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_en13555pqrdeltaegc.h"

STD_EN13555PqrDeltaeGC::STD_EN13555PqrDeltaeGC(
                    const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_EN13555PqrDeltaeGC::STD_EN13555PqrDeltaeGC(STD_EN13555PqrDeltaeGC* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

STD_EN13555PqrDeltaeGC::~STD_EN13555PqrDeltaeGC() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li qa gasket stress at assembly [N/mm2],
 * the highest one at temperature is Qsmax
 * \li temp temperature [Celsius]
 * \li pqr Pqr [-]
 * \li deltaegc delta eGC [mm]
 */
void STD_EN13555PqrDeltaeGC::createMembers() {
    addMember("temp", "Celsius", 0.0, RB2::MemberDouble);
    addMember("qg", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("pqr", "-", 0.0, RB2::MemberDouble);
    addMember("deltaegc", "mm", 0.0, RB2::MemberDouble);
}
