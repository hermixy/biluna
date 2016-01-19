/*****************************************************************
 * $Id: std_egeg.cpp 0001 2016-01-06T18:00:12 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_egeg.h"

STD_EGeG::STD_EGeG(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_EGeG::STD_EGeG(STD_EGeG* obj) : RB_ObjectContainer(obj) {
	createMembers();
	*this = *project;
}

STD_EGeG::~STD_EGeG() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_EGeG::createMembers() {
    addMember("qg", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("capeg", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("eg", "mm", 0.0, RB2::MemberDouble);
}
