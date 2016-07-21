/*****************************************************************
 * $Id: std_flangefacingdimasme.cpp 0001 2016-07-21T14:19:46 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flangefacingdimasme.h"

STD_FlangeFacingDimAsme::STD_FlangeFacingDimAsme(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_FlangeFacingDimAsme::STD_FlangeFacingDimAsme(STD_FlangeFacingDimAsme* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_FlangeFacingDimAsme::~STD_FlangeFacingDimAsme() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li rating rating of flange
 * \li nomsize nominal diameter
 * \li r outside diameter raised face large male and large tongue
 * \li s outside diameter small male (note 1)
 * \li t outside diameter small tongue
 * \li u inside diameter of large and small tongue
 * \li idt inside diameter of small tongue (note 1)
 * \li w outside diameter large female and large groove
 * \li x outside diameter small female
 * \li y outside diameter small groove
 * \li z inside diameter of large and small groove
 * \li hrf height raised face (note 2, 3)
 * \li htg height large and small male and tongue (note 2, 4)
 * \li depthg depth of groove or female (note 2, 5)
 * \li k minimum outside diameter of raised portion (note 6, 7) small female and groove
 * \li l minimum outside diameter of raised portion (note 6, 7) large female and groove
 */
void STD_FlangeFacingDimAsme::createMembers() {
    addMember("rating", "Pound", 0, RB2::MemberInteger);
    addMember("nomsize", "NPS", 0, RB2::MemberDouble);
    addMember("r", "mm", 0, RB2::MemberDouble);
    addMember("s", "mm", 0, RB2::MemberDouble);
    addMember("t", "mm", 0, RB2::MemberDouble);
    addMember("u", "mm", 0, RB2::MemberDouble);
    addMember("idt", "mm", 0, RB2::MemberDouble);
    addMember("w", "mm", 0, RB2::MemberDouble);
    addMember("x", "mm", 0, RB2::MemberDouble);
    addMember("y", "mm", 0, RB2::MemberDouble);
    addMember("z", "mm", 0, RB2::MemberDouble);
    addMember("hrf", "mm", 0, RB2::MemberDouble);
    addMember("htg", "mm", 0, RB2::MemberDouble);
    addMember("depthg", "mm", 0, RB2::MemberDouble);
    addMember("k", "mm", 0, RB2::MemberDouble);
    addMember("l", "mm", 0, RB2::MemberDouble);
}
