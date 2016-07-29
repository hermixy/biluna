/*****************************************************************
 * $Id: std_comprating.cpp 0001 2016-07-20T15:54:50 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_comprating.h"

STD_CompRating::STD_CompRating(const QString& id, RB_ObjectBase* p,
                       const QString& n, RB_ObjectFactory* f)
                        : RB_ObjectContainer(id, p, n, f) {
	createMembers();
}

STD_CompRating::STD_CompRating(STD_CompRating* obj) :
                   RB_ObjectContainer(obj) {
	createMembers();
	*this = *obj;
}

STD_CompRating::~STD_CompRating() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li rating rating or schedule, NPS or Class for flange
 */
void STD_CompRating::createMembers() {
    addMember("rating", "-", 0.0, RB2::MemberDouble);
}
