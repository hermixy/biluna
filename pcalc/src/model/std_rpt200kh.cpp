/*****************************************************************
 * $Id: std_rpt200kh.cpp 0001 2016-05-21T10:06:33 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_rpt200kh.h"

STD_RpT200kh::STD_RpT200kh(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_RpT200kh::STD_RpT200kh(STD_RpT200kh* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_RpT200kh::~STD_RpT200kh() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li rp stress [MPa]
 */
void STD_RpT200kh::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("rpt200kh", "MPa", 0.0, RB2::MemberDouble);

    // temporary for importing of external data only
    addMember("tmp_id", "-", "0", RB2::MemberChar125);
    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
