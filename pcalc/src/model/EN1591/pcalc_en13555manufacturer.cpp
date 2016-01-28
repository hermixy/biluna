/*****************************************************************
 * $Id: pcalc_en13555manufacturer.cpp 0001 2016-01-25T21:20:35 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_en13555manufacturer.h"

PCALC_En13555Manufacturer::PCALC_En13555Manufacturer(
                    const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

PCALC_En13555Manufacturer::PCALC_En13555Manufacturer(
                    PCALC_En13555Manufacturer* obj)
                    : RB_ObjectContainer(obj) {
	createMembers();
    *this = *obj;
}

PCALC_En13555Manufacturer::~PCALC_En13555Manufacturer() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void PCALC_En13555Manufacturer::createMembers() {
    addMember("manufacturer", "-", "<NEW>", RB2::MemberChar125);
    addMember("website", "-", "", RB2::MemberChar125);
}
