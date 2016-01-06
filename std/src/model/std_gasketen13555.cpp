/*****************************************************************
 * $Id: std_gasketen.cpp 0001 2016-01-06T17:05:37 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_gasketen13555.h"

STD_GasketEN13555::STD_GasketEN13555(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_GasketEN13555::STD_GasketEN13555(STD_GasketEN* obj) : RB_ObjectContainer(obj) {
	createMembers();
	*this = *project;
}

STD_GasketEN13555::~STD_GasketEN13555() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_GasketEN13555::createMembers() {
    addMember("manufname", "-", "<NEW>", RB2::MemberChar40);
    addMember("manuftype", "-", "", RB2::MemberChar40);
    addMember("manufsubtype", "-", "", RB2::MemberChar40);
    addMember("testoutsidediam", "mm", 0, RB2::MemberDouble);
    addMember("testinsidediam", "mm", 0, RB2::MemberDouble);
}
