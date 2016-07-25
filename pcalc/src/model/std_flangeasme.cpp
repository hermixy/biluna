/*****************************************************************
 * $Id: std_flangeasme.cpp 0001 2016-07-21T14:41:36 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_flangeasme.h"

STD_FlangeAsme::STD_FlangeAsme(
                   const QString& id, RB_ObjectBase* p,
                   const QString& n, RB_ObjectFactory* f)
					: RB_ObjectAtomic(id, p, n, f) {
	createMembers();
}

STD_FlangeAsme::STD_FlangeAsme(STD_FlangeAsme* obj) : 
                   RB_ObjectAtomic(obj) {
	createMembers();
	*this = *obj;
}

STD_FlangeAsme::~STD_FlangeAsme() {
	// clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * \li rating flange pressure rating
 * \li nomsize nominal pipe size
 * \li o outside diameter of flange
 * \li tf1 minimum thickness of flange (note 2 - 4)
 * \li tf2 minimum thickness of lap joint
 * \li x diameter of hub
 * \li ah hub diameter beginning of chamfer welding, neck (note 4)
 * \li y1 lenght through hub threaded/slip-on/socket welding
 * \li y2 length through hub lapped
 * \li y3 length through hub welding neck
 * \li t minimum thread length threaded
 * \li b1 bore minimum slip-on/socket welding
 * \li b2 bore minimum lapped
 * \li b3 bore welding neck/socket welding (note 6)
 * \li r corner radius of bore of lapped flange and pipe
 * \li r1 minimum filet radius, only ASME B16.47
 * \li q minimum counter-bore threaded flange
 * \li d depth of socket
 * \li w diameter of bolt circle
 * \li diambh diameter of bolt holes [inch]
 * \li nob number of bolts
 * \li diamb diameter of bolts [inch]
 * \li l1 length of bolts stud bolts 2 mm raised face (note 1, 4)
 * \li l2 length of bolts stud bolts ring joint (note 1, 4)
 * \li l3 length of bolts machine bolts 2 mm raised face
 */
void STD_FlangeAsme::createMembers() {
    addMember("rating", "Pound", 0, RB2::MemberInteger);
    addMember("nomsize", "NPS", 0, RB2::MemberDouble);
    addMember("o", "mm", 0, RB2::MemberDouble);
    addMember("tf1", "mm", 0, RB2::MemberDouble);
    addMember("tf2", "mm", 0, RB2::MemberDouble);
    addMember("x", "mm", 0, RB2::MemberDouble);
    addMember("ah", "mm", 0, RB2::MemberDouble);
    addMember("y1", "mm", 0, RB2::MemberDouble);
    addMember("y2", "mm", 0, RB2::MemberDouble);
    addMember("y3", "mm", 0, RB2::MemberDouble);
    addMember("t", "mm", 0, RB2::MemberDouble);
    addMember("b1", "mm", 0, RB2::MemberDouble);
    addMember("b2", "mm", 0, RB2::MemberDouble);
    addMember("b3", "mm", 0, RB2::MemberDouble);
    addMember("r", "mm", 0, RB2::MemberDouble);
    addMember("r1", "mm", 0, RB2::MemberDouble);
    addMember("q", "mm", 0, RB2::MemberDouble);
    addMember("d", "mm", 0, RB2::MemberDouble);
    addMember("w", "mm", 0, RB2::MemberDouble);
    addMember("diambh", "inch", 0, RB2::MemberDouble);
    addMember("nob", "-", 0, RB2::MemberInteger);
    addMember("diamb", "inch", 0, RB2::MemberDouble);
    addMember("l1", "mm", 0, RB2::MemberDouble);
    addMember("l2", "mm", 0, RB2::MemberDouble);
    addMember("l3", "mm", 0, RB2::MemberDouble);
}
