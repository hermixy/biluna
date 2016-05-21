/*****************************************************************
 * $Id: std_rp02.cpp 0001 2016-05-21T13:37:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_rp02.h"

STD_Rp02::STD_Rp02(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_Rp02::STD_Rp02(STD_Rp02* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_Rp02::~STD_Rp02() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li temperature temperature [Celsius]
 * \li rp02 0.2% proof strength
 */
void STD_Rp02::createMembers() {
    addMember("temperature", "Celsius", 0.0, RB2::MemberDouble);
    addMember("rp02", "MPa", 0.0, RB2::MemberDouble);
}
