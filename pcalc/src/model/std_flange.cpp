/*****************************************************************
 * $Id: std_flange.cpp 0001 2016-07-07T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flange.h"

STD_Flange::STD_Flange(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic (id, p, n, f) {
	createMembers();
}

STD_Flange::STD_Flange(STD_Flange* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_Flange::~STD_Flange() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li nomdiam DN or NPS nominal diameter
 * \li d outside diameter
 * \li k diameter of bolt circle
 * \li l diameter of bolt hole
 * \li number number of bolts
 * \li size nominal size of bolt M value or Inch
 * \li a outside diameter of neck
 * \li b1 bore diameter 1
 * \li b2 bore diameter 2
 * \li b3 bore diameter 3
 * \li c1 flange thickness 1
 * \li c2 flange thickness 2
 * \li c3 flange thickness 3
 * \li c4 flange thickness 4
 * \li e chamfer
 * \li f1 collar thickness 1
 * \li f2 collar thickness 2
 * \li f3 collar thickness 3
 * \li f4 collar thickness 4
 * \li gmax diameter of shoulder
 * \li h1 length 1
 * \li h2 length 2
 * \li h3 length 3
 * \li h4 length 4
 * \li h5 length 5
 * \li n1 neck diameter 1
 * \li n2 neck diameter 2
 * \li n3 neck diameter 3
 * \li r1 corner radius
 * \li s wall thickness
 */
void STD_Flange::createMembers() {
    addMember("nomdiam", "-", 0, RB2::MemberDouble);
    addMember("d", "mm", 0, RB2::MemberDouble);
    addMember("k", "mm", 0, RB2::MemberDouble);
    addMember("l", "mm", 0, RB2::MemberDouble);
    addMember("number", "-", 0, RB2::MemberInteger);
    addMember("size", "-", "", RB2::MemberChar20);
    addMember("a", "mm", 0, RB2::MemberDouble);
    addMember("b1", "mm", 0, RB2::MemberDouble);
    addMember("b2", "mm", 0, RB2::MemberDouble);
    addMember("b3", "mm", 0, RB2::MemberDouble);
    addMember("c1", "mm", 0, RB2::MemberDouble);
    addMember("c2", "mm", 0, RB2::MemberDouble);
    addMember("c3", "mm", 0, RB2::MemberDouble);
    addMember("c4", "mm", 0, RB2::MemberDouble);
    addMember("e", "mm", 0, RB2::MemberDouble);
    addMember("f1", "mm", 0, RB2::MemberDouble);
    addMember("f2", "mm", 0, RB2::MemberDouble);
    addMember("f3", "mm", 0, RB2::MemberDouble);
    addMember("f4", "mm", 0, RB2::MemberDouble);
    addMember("gmax", "mm", 0, RB2::MemberDouble);
    addMember("h1", "mm", 0, RB2::MemberDouble);
    addMember("h2", "mm", 0, RB2::MemberDouble);
    addMember("h3", "mm", 0, RB2::MemberDouble);
    addMember("h4", "mm", 0, RB2::MemberDouble);
    addMember("h5", "mm", 0, RB2::MemberDouble);
    addMember("n1", "mm", 0, RB2::MemberDouble);
    addMember("n2", "mm", 0, RB2::MemberDouble);
    addMember("n3", "mm", 0, RB2::MemberDouble);
    addMember("r1", "mm", 0, RB2::MemberDouble);
    addMember("s", "mm", 0, RB2::MemberDouble);
}
