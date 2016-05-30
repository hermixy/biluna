/*****************************************************************
 * $Id: std_thermexpvalue.cpp 0001 2016-05-21T13:04:41 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_thermexp.h"

STD_ThermExp::STD_ThermExp(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_ThermExp::STD_ThermExp(STD_ThermExp* obj) :
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_ThermExp::~STD_ThermExp() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li thermexp thermal expansion [1/Kelvin]
 */
void STD_ThermExp::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("thermexp", "1/Kelvin", 0.0, RB2::MemberDouble);

    // temporary for importing of external data only
    addMember("tmp_id", "-", "0", RB2::MemberChar125);
    addMember("tmp_parent", "-", "0", RB2::MemberChar125);
}
