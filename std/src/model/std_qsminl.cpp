/*****************************************************************
 * $Id: std_qsminl.cpp 0001 2016-01-06T17:30:59 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_qsminl.h"

STD_QsminL::STD_QsminL(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_QsminL::STD_QsminL(STD_QsminL* obj) : RB_ObjectContainer(obj) {
	createMembers();
	*this = *project;
}

STD_QsminL::~STD_QsminL() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_QsminL::createMembers() {
    addMember("testp", "bar", 0.0, RB2::MemberDouble);
    addMember("leakrate", "-", 0.0, RB2::MemberDouble);
    addMember("qa", "N/mm2", 0.0, RB2::MemberDouble);
    addMember("qsminl", "N/mm2", 0.0, RB2::MemberDouble);
}
