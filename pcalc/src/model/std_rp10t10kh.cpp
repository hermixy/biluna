/*****************************************************************
 * $Id: std_rp10t10kh.cpp 0001 2016-05-21T09:57:18 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_rp10t10kh.h"

STD_Rp10T10kh::STD_Rp10T10kh(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_Rp10T10kh::STD_Rp10T10kh(STD_Rp10T10kh* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_Rp10T10kh::~STD_Rp10T10kh() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li rp10 stress [MPa]
 */
void STD_Rp10T10kh::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("rp10t10kh", "MPa", 0.0, RB2::MemberDouble);

    // temporary for importing of external data only
    addMember("tmp_id", "-", "0", RB2::MemberChar125);
    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
