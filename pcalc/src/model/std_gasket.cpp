/*****************************************************************
 * $Id: std_gasket.cpp 0001 2016-08-02T19:29:18 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_gasket.h"

STD_Gasket::STD_Gasket(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_Gasket::STD_Gasket(STD_Gasket* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_Gasket::~STD_Gasket() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li stdname dimension standard name
 * \li typename gasket type name as per standard
 * \li rating pressure rating of the gasket in PN or Pounds
 * \li nomsize nominal size of gasket in DN or NPS
 * \li flangestdname name of corresponding flange standard
 * \li number gasket number as per standard
 * \li d0 outside diameter of gasket sealing part
 * \li d1 inside diameter of gasket sealing part
 * \li d2 outside diameter of outer guide ring
 * \li d3 inside diameter of inner guide ring
 * \li d4 
 * \li d5 
 * \li d6 
 * \li nobolts number of bolts
 * \li t0 thickness of gasket sealing part
 * \li t1 thickness of inner guide ring
 * \li t2 thickness of outer guide ring
 * \li nogrooves number of grooves
 * \li w0 
 * \li w1 
 * \li r0 
 */
void STD_Gasket::createMembers() {
    addMember("stdname", "-", "<NEW>", RB2::MemberChar255);
    addMember("typename", "-", "", RB2::MemberChar255);
    addMember("rating", "-", 0.0, RB2::MemberDouble);
    addMember("nomsize", "-", 0.0, RB2::MemberDouble);
    addMember("flangestdname", "-", "", RB2::MemberChar255);
    addMember("number", "-", "", RB2::MemberChar255);
    addMember("d0", "mm", 0.0, RB2::MemberDouble);
    addMember("d1", "mm", 0.0, RB2::MemberDouble);
    addMember("d2", "mm", 0.0, RB2::MemberDouble);
    addMember("d3", "mm", 0.0, RB2::MemberDouble);
    addMember("d4", "mm", 0.0, RB2::MemberDouble);
    addMember("d5", "mm", 0.0, RB2::MemberDouble);
    addMember("d6", "mm", 0.0, RB2::MemberDouble);
    addMember("nobolts", "mm", 0, RB2::MemberInteger);
    addMember("t0", "mm", 0.0, RB2::MemberDouble);
    addMember("t1", "mm", 0.0, RB2::MemberDouble);
    addMember("t2", "mm", 0.0, RB2::MemberDouble);
    addMember("nogrooves", "mm", 0.0, RB2::MemberDouble);
    addMember("w0", "mm", 0.0, RB2::MemberDouble);
    addMember("w1", "mm", 0.0, RB2::MemberDouble);
    addMember("r0", "mm", 0.0, RB2::MemberDouble);
}
