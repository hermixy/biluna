/*****************************************************************
 * $Id: std_rp10.cpp 0001 2016-05-21T13:34:38 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_rp10.h"

STD_Rp10::STD_Rp10(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_Rp10::STD_Rp10(STD_Rp10* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_Rp10::~STD_Rp10() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li rp10 1% proof strength
 */
void STD_Rp10::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("rp10", "MPa", 0.0, RB2::MemberDouble);

    // temporary for importing of external data only
//    addMember("tmp_id", "-", "0", RB2::MemberChar125);
//    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
