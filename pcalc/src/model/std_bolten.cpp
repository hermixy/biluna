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
 * \li pa thread pitch
 * \li brefb
 * \li brefc
 * \li brefd
 * \li cmax
 * \li cmin
 * \li damax
 * \li dsnom nominal diameter
 * \li dsa
 * \li dsb
 * \li dwa
 * \li dwb
 * \li ea
 * \li eb
 * \li lfmax
 * \li knom
 * \li kamax
 * \li kamin
 * \li kbmax
 * \li kbmin
 * \li kwa
 * \li kwb
 * \li rmin
 * \li snom
 * \li samin
 * \li sbmin
 */
void STD_BoltEn::createMembers() {
    addMember("nomsize", "-", "<NEW>", RB2::MemberChar10);
    addMember("pa", "mm", 0, RB2::MemberDouble);
    addMember("brefb", "mm", 0, RB2::MemberDouble);
    addMember("brefc", "mm", 0, RB2::MemberDouble);
    addMember("brefd", "mm", 0, RB2::MemberDouble);
    addMember("cmax", "mm", 0, RB2::MemberDouble);
    addMember("cmin", "mm", 0, RB2::MemberDouble);
    addMember("damax", "mm", 0, RB2::MemberDouble);
    addMember("dsnom", "mm", 0, RB2::MemberDouble);
    addMember("dsa", "mm", 0, RB2::MemberDouble);
    addMember("dsb", "mm", 0, RB2::MemberDouble);
    addMember("dwa", "mm", 0, RB2::MemberDouble);
    addMember("dwb", "mm", 0, RB2::MemberDouble);
    addMember("ea", "mm", 0, RB2::MemberDouble);
    addMember("eb", "mm", 0, RB2::MemberDouble);
    addMember("lfmax", "mm", 0, RB2::MemberDouble);
    addMember("knom", "mm", 0, RB2::MemberDouble);
    addMember("kamax", "mm", 0, RB2::MemberDouble);
    addMember("kamin", "mm", 0, RB2::MemberDouble);
    addMember("kbmax", "mm", 0, RB2::MemberDouble);
    addMember("kbmin", "mm", 0, RB2::MemberDouble);
    addMember("kwa", "mm", 0, RB2::MemberDouble);
    addMember("kwb", "mm", 0, RB2::MemberDouble);
    addMember("rmin", "mm", 0, RB2::MemberDouble);
    addMember("snom", "mm", 0, RB2::MemberDouble);
    addMember("samin", "mm", 0, RB2::MemberDouble);
    addMember("sbmin", "mm", 0, RB2::MemberDouble);
}
