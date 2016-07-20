/*****************************************************************
 * $Id: std_rating.cpp 0001 2016-07-20T15:54:50 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_rating.h"

STD_Rating::STD_Rating(const QString& id, RB_ObjectBase* p,
                       const QString& n, RB_ObjectFactory* f)
                        : RB_ObjectContainer(id, p, n, f) {
	createMembers();
}

STD_Rating::STD_Rating(STD_Rating* obj) : 
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_Rating::~STD_Rating() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li rating rating or schedule, NPS or Class for flange
 */
void STD_Rating::createMembers() {
    addMember("rating", "-", 0.0, RB2::MemberDouble);
}
