/*****************************************************************
 * $Id: pcalc_classgenerator.cpp 0001 2016-05-09T08:15:35 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "pcalc_classgenerator.h"

PCALC_ClassGenerator::PCALC_ClassGenerator(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

PCALC_ClassGenerator::PCALC_ClassGenerator(PCALC_ClassGenerator* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

PCALC_ClassGenerator::~PCALC_ClassGenerator() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li codename Name of calculation code
 * \li subname Sub name or paragraph
 * \li issue Issue date and/or revision number
 * \li description Short description of this code part
 */
void PCALC_ClassGenerator::createMembers() {
    addMember("codename", "-", "", RB2::MemberChar125);
    addMember("subname", "-", "", RB2::MemberChar125);
    addMember("issue", "-", "", RB2::MemberChar125);
    addMember("description", "-", "", RB2::MemberChar255);
}
