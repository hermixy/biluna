/*****************************************************************
 * $Id: std_elasmodulvalue.cpp 0001 2016-05-21T11:58:22 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_elasmodul.h"

STD_ElasModul::STD_ElasModul(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_ElasModul::STD_ElasModul(STD_ElasModul* obj) :
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_ElasModul::~STD_ElasModul() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li elasmodul modulus of elasticity [N/mm2]
 */
void STD_ElasModul::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("elasmodul", "N/mm2", 0.0, RB2::MemberDouble);

    // temporary for importing of external data only
    addMember("tmp_id", "-", "0", RB2::MemberChar125);
    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
