/*****************************************************************
 * $Id: std_bolten.cpp 0001 2016-07-31T15:05:32 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_bolten.h"

STD_BoltEn::STD_BoltEn(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_BoltEn::STD_BoltEn(STD_BoltEn* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_BoltEn::~STD_BoltEn() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li nomsize nominal bolt size, e.g.: M8 M10 M30
 * \li pitch thread pitch
 * \li d basic diameter
 * \li e nut width accross corners
 * \li m nut thickness
 * \li s nut thickness accros flats
 */
void STD_BoltEn::createMembers() {
    addMember("nomsize", "-", "", RB2::MemberChar10);
    addMember("pitch", "mm", 0, RB2::MemberDouble);
    addMember("d", "mm", 0, RB2::MemberDouble);
    // TODO: actually nut values
    addMember("e", "mm", 0, RB2::MemberDouble);
    addMember("m", "mm", 0, RB2::MemberDouble);
    addMember("s", "mm", 0, RB2::MemberDouble);
}
