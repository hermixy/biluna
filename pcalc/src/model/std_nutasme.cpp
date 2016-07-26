/*****************************************************************
 * $Id: std_nutasme.cpp 0001 2016-07-26T20:48:47 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_nutasme.h"

STD_NutAsme::STD_NutAsme(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_NutAsme::STD_NutAsme(STD_NutAsme* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_NutAsme::~STD_NutAsme() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li nomsize nominal size or basic major diameter of thread
 * \li fbasic width across flats F basic
 * \li fmax width across flats F maximum
 * \li fmin width across flats F minimum
 * \li gmax width across corners G maximum
 * \li gmin width across corners G minimum
 * \li hbasic thickness nut basic
 * \li hmax thickness nut maximum
 * \li hmin thickness nut minimum
 * \li h1basic thickness jam nut basic
 * \li h1max thickness jam nut maximum
 * \li h1min thickness jam nut minimum
 */
void STD_NutAsme::createMembers() {
    addMember("nomsize", "inch", 0.0, RB2::MemberDouble);
    addMember("fbasic", "inch", 0.0, RB2::MemberDouble);
    addMember("fmax", "inch", 0.0, RB2::MemberDouble);
    addMember("fmin", "inch", 0.0, RB2::MemberDouble);
    addMember("gmax", "inch", 0.0, RB2::MemberDouble);
    addMember("gmin", "inch", 0.0, RB2::MemberDouble);
    addMember("hbasic", "inch", 0.0, RB2::MemberDouble);
    addMember("hmax", "inch", 0.0, RB2::MemberDouble);
    addMember("hmin", "inch", 0.0, RB2::MemberDouble);
    addMember("h1basic", "inch", 0.0, RB2::MemberDouble);
    addMember("h1max", "inch", 0.0, RB2::MemberDouble);
    addMember("h1min", "inch", 0.0, RB2::MemberDouble);
}
