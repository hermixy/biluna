/*****************************************************************
 * $Id: std_nuten.cpp 0001 2016-07-31T15:11:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_nuten.h"

STD_NutEn::STD_NutEn(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_NutEn::STD_NutEn(STD_NutEn* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_NutEn::~STD_NutEn() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li nomsize nominal size, e.g.: M8 M10 M30
 * \li pa pitch of thread
 * \li cmax
 * \li cmin
 * \li damax
 * \li damin
 * \li dwmin
 * \li emin minimum width accross corners
 * \li mnom nominal and maximum thickness
 * \li mmin minimum thickness
 * \li mwmin minimum wrench thickness
 * \li snom nominal and maximum width accross flats
 * \li smin minimum width accross flats
 */
void STD_NutEn::createMembers() {
    addMember("nomsize", "-", "<NEW>", RB2::MemberChar10);
    addMember("pa", "mm", 0, RB2::MemberDouble);
    addMember("cmax", "mm", 0, RB2::MemberDouble);
    addMember("cmin", "mm", 0, RB2::MemberDouble);
    addMember("damax", "mm", 0, RB2::MemberDouble);
    addMember("damin", "mm", 0, RB2::MemberDouble);
    addMember("dwmin", "mm", 0, RB2::MemberDouble);
    addMember("emin", "mm", 0, RB2::MemberDouble);
    addMember("mnom", "mm", 0, RB2::MemberDouble);
    addMember("mmin", "mm", 0, RB2::MemberDouble);
    addMember("mwmin", "mm", 0, RB2::MemberDouble);
    addMember("snom", "mm", 0, RB2::MemberDouble);
    addMember("smin", "mm", 0, RB2::MemberDouble);
}
