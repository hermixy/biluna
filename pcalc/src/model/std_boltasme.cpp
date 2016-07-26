/*****************************************************************
 * $Id: std_boltasme.cpp 0001 2016-07-26T20:39:50 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_boltasme.h"

STD_BoltAsme::STD_BoltAsme(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_BoltAsme::STD_BoltAsme(STD_BoltAsme* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_BoltAsme::~STD_BoltAsme() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li nomsize Nominal size and basic major diameter D
 * \li thr Threads per inch
 * \li d2 Basic pitch diameter
 * \li d3 UNR design minor diameter external
 * \li d1 Basic minor diameter internal
 * \li lambda1 Lead angle at basic pitch degree
 * \li lambda2 Lead angle at basic pitch minutes
 * \li d2hb Section at minor diamter at D - 2hb
 * \li area Tensile stress area
 */
void STD_BoltAsme::createMembers() {
    addMember("nomsize", "inch", 0.0, RB2::MemberDouble);
    addMember("thr", "1/inch", 0.0, RB2::MemberDouble);
    addMember("d2", "inch", 0.0, RB2::MemberDouble);
    addMember("d3", "inch", 0.0, RB2::MemberDouble);
    addMember("d1", "inch", 0.0, RB2::MemberDouble);
    addMember("lambda1", "degree", 0.0, RB2::MemberDouble);
    addMember("lambda2", "minutes", 0.0, RB2::MemberDouble);
    addMember("d2hb", "inch2", 0.0, RB2::MemberDouble);
    addMember("area", "inch2", 0.0, RB2::MemberDouble);
}
