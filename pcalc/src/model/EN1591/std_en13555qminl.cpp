/*****************************************************************
 * $Id: std_en13555qminl.cpp 0001 2016-01-24T16:31:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_en13555qminl.h"

STD_EN13555QminL::STD_EN13555QminL(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_EN13555QminL::STD_EN13555QminL(STD_EN13555QminL* obj)
                    : RB_ObjectAtomic(obj) {
	createMembers();
    *this = *obj;
}

STD_EN13555QminL::~STD_EN13555QminL() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_EN13555QminL::createMembers() {
    addMember("testpress", "Bar", 0.0, RB2::MemberDouble);
    addMember("leakrate", "-", 0.0, RB2::MemberDouble);
    addMember("qminl", "N/mm2", 0.0, RB2::MemberDouble);

    // temporary for importing of external data only
//    addMember("tmp_id", "-", "0", RB2::MemberChar125);
//    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
