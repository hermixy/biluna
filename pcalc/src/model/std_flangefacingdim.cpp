/*****************************************************************
 * $Id: std_flangefacingdim.cpp 0001 2016-07-13T15:43:00 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flangefacingdim.h"

STD_FlangeFacingDim::STD_FlangeFacingDim(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_FlangeFacingDim::STD_FlangeFacingDim(STD_FlangeFacingDim* obj) :
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_FlangeFacingDim::~STD_FlangeFacingDim() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li type facing type number or name
 * \li description description of the facing type
 */
void STD_FlangeFacingDim::createMembers() {
    addMember("rating", "NPS", 0.0, RB2::MemberDouble);
    addMember("nomdiam", "DN", 0.0, RB2::MemberDouble);
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
