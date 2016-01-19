/*****************************************************************
 * $Id: std_pqrdeltaegc.cpp 0001 2016-01-06T17:56:03 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_pqrdeltaegc.h"

STD_PqrDeltaeGc::STD_PqrDeltaeGc(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_PqrDeltaeGc::STD_PqrDeltaeGc(STD_PqrDeltaeGc* obj) : RB_ObjectContainer(obj) {
	createMembers();
	*this = *project;
}

STD_PqrDeltaeGc::~STD_PqrDeltaeGc() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_PqrDeltaeGc::createMembers() {
    addMember("qg", "-", 0.0, RB2::MemberDouble);
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("pqr", "-", 0.0, RB2::MemberDouble);
    addMember("deltaegc", "mm", 0.0, RB2::MemberDouble);
}
