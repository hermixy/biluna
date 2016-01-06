/*****************************************************************
 * $Id: std_project.cpp 0001 2016-01-06T20:31:37 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_project.h"

STD_Project::STD_Project(const QString& id, RB_ObjectBase* p,
                    const QString& n, RB_ObjectFactory* f)
					: RB_ObjectContainer (id, p, n, f) {
	createMembers();
}

STD_Project::STD_Project(STD_Project* obj) : RB_ObjectContainer(obj) {
	createMembers();
	*this = *project;
}

STD_Project::~STD_Project() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

void STD_Project::createMembers() {
    addMember("number", "-", "STD0001", RB2::MemberChar125);
    addMember("description", "-", "Default", RB2::MemberChar255);
    addMember("company", "-", "Biluna", RB2::MemberChar125);
    addMember("location", "-", "The Netherlands", RB2::MemberChar125);
}
