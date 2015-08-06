/*****************************************************************
 * $Id: scan_block.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-05-04 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_block.h"



SCAN_Block::SCAN_Block(const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
                        : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SCAN_Block::SCAN_Block(SCAN_Block* b) : RB_ObjectContainer(b) {
    createMembers();
    *this = *b;
}

SCAN_Block::~SCAN_Block () {
    // deletion of children and members is done in RB_ObjectBase
    // and RB_ObjectContainer
}

/**
 * Create members:
 * number sequence number for sorting
 * blocktext block text with {$image_id} to position image
 * width image boundary width
 * height image boundary height
 * var1Type variable 1 type, can be an answer
 *   or a complex result from resulthelper for example:
 *   Jerry Rhodes Total 01 (JrTotal01)
 * var1Min minimum value for variable 1 to be valid
 * var1Max maximum value for variable 1 to be valid
 * andor and/or other minimum and maximum values of other variables
 * etc.
 */
void SCAN_Block::createMembers() {
    addMember("number", "-", 1, RB2::MemberInteger);
    addMember("blocktext", "-", "Block text ...", RB2::MemberString);
    addMember("contenttype", "-", 0, RB2::MemberInteger);
    addMember("var1type", "-", 0, RB2::MemberInteger);
    addMember("var1min", "-", 0, RB2::MemberInteger);
    addMember("var1max", "-", 0, RB2::MemberInteger);
    addMember("andor1", "-", 0, RB2::MemberInteger);
    addMember("var2type", "-", 0, RB2::MemberInteger);
    addMember("var2min", "-", 0, RB2::MemberInteger);
    addMember("var2max", "-", 0, RB2::MemberInteger);
    addMember("andor2", "-", 0, RB2::MemberInteger);
    addMember("var3type", "-", 0, RB2::MemberInteger);
    addMember("var3min", "-", 0, RB2::MemberInteger);
    addMember("var3max", "-", 0, RB2::MemberInteger);
}
