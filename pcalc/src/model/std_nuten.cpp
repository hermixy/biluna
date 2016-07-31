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
 * \li pitch pitch of thread
 * \li smax maximum width accross flats
 * \li smin minimum width accross flats
 * \li emin minimum width accross corners
 * \li mmax maximum thickness
 * \li mmin minimum thickness
 * \li tol tolerance on squareness
 */
void STD_NutEn::createMembers() {
    addMember("nomsize", "-", "", RB2::MemberChar10);
    addMember("pitch", "mm", 0, RB2::MemberDouble);
    addMember("smax", "mm", 0, RB2::MemberDouble);
    addMember("smin", "mm", 0, RB2::MemberDouble);
    addMember("emin", "mm", 0, RB2::MemberDouble);
    addMember("mmax", "mm", 0, RB2::MemberDouble);
    addMember("mmin", "mm", 0, RB2::MemberDouble);
    addMember("tol", "mm", 0, RB2::MemberDouble);
}
