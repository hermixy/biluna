/*****************************************************************
 * $Id: std_flangetypelimit.cpp 0001 2016-07-14T14:22:22 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flangetypelimit.h"

STD_FlangeTypeLimit::STD_FlangeTypeLimit(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_FlangeTypeLimit::STD_FlangeTypeLimit(STD_FlangeTypeLimit* obj) :
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_FlangeTypeLimit::~STD_FlangeTypeLimit() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li rating flange rating PN or Pound Class
 * \li minsize minimum flange size
 * \li maxsize maximum flange size
 */
void STD_FlangeTypeLimit::createMembers() {
    addMember("rating", "-", 0.0, RB2::MemberDouble);
    addMember("minsize", "-", 0.0, RB2::MemberDouble);
    addMember("maxsize", "-", 0.0, RB2::MemberDouble);
}
