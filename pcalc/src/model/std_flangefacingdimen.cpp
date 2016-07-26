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
 * \li d1
 * \li f1
 * \li f2
 * \li f3
 * \li f4
 * \li w
 * \li x
 * \li y
 * \li z
 * \li alpha
 * \li r
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
