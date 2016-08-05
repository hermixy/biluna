/*****************************************************************
 * $Id: std_flangefacingdimen.cpp 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flangefacingdimen.h"

STD_FlangeFacingDimEn::STD_FlangeFacingDimEn(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_FlangeFacingDimEn::STD_FlangeFacingDimEn(STD_FlangeFacingDimEn* obj) :
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_FlangeFacingDimEn::~STD_FlangeFacingDimEn() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li rating
 * \li nomsize nominal diameter
 * \li d1 outside diameter raised face, and (female) groove and recess
 * \li f1 height raised face type B, and (female) groove and recess
 * \li f2 height tongue and depth O-ring spigot type C (male) and G (female)
 * \li f3 depth of groove, recess and O-ring groove type D, F and H
 * \li f4 height of O-ring groove male part type H
 * \li w inside diameter tongue and O-ring spigot type C and G
 * \li x outside diameter tongue and spigot type C and E
 * \li y outside diameter female part groove, recess and O-ring groove type D, F and H
 * \li z inside diameter female part groove and O-ring groove type D and H
 * \li alpha angle of O-ring groove
 * \li r radius of O-ring groove
 */
void STD_FlangeFacingDimEn::createMembers() {
    addMember("rating", "NPS", 0.0, RB2::MemberDouble);
    addMember("nomsize", "DN", 0.0, RB2::MemberDouble);
    addMember("d1", "mm", 0.0, RB2::MemberDouble);
    addMember("f1", "mm", 0.0, RB2::MemberDouble);
    addMember("f2", "mm", 0.0, RB2::MemberDouble);
    addMember("f3", "mm", 0.0, RB2::MemberDouble);
    addMember("f4", "mm", 0.0, RB2::MemberDouble);
    addMember("w", "mm", 0.0, RB2::MemberDouble);
    addMember("x", "mm", 0.0, RB2::MemberDouble);
    addMember("y", "mm", 0.0, RB2::MemberDouble);
    addMember("z", "mm", 0.0, RB2::MemberDouble);
    addMember("alpha", "degree", 0.0, RB2::MemberDouble);
    addMember("r", "mm", 0.0, RB2::MemberDouble);
}
