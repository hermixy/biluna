/*****************************************************************
 * $Id: std_qminl.cpp 0001 2016-01-06T17:16:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_qminl.h"

STD_QminL::STD_QminL(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_QminL::STD_QminL(STD_QminL* obj) : RB_ObjectContainer(obj) {
	createMembers();
	*this = *project;
}

STD_QminL::~STD_QminL() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_QminL::createMembers() {
    addMember("testp", "bar", 0.0, RB2::MemberDouble);
    addMember("leakrate", "-", 0.0, RB2::MemberDouble);
    addMember("qminl", "N/mm2", 0.0, RB2::MemberDouble);
}
