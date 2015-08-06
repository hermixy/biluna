/*****************************************************************
 * $Id: scan_answer.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_answer.h"

SCAN_Answer::SCAN_Answer (const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
                        : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SCAN_Answer::SCAN_Answer(SCAN_Answer* answer)
                                    : RB_ObjectContainer(answer) {
    createMembers();
    *this = *answer;
}

SCAN_Answer::~SCAN_Answer () {
    // deletion of children and members is done in RB_ObjectBase
    // and RB_ObjectContainer
}

/**
 * Create member:
 * number sequence number of answer in the multiple choice
 * answer answer text
 */
void SCAN_Answer::createMembers() {
    addMember("number", "-", 1, RB2::MemberInteger);
    addMember("answer", "-", "", RB2::MemberString);
}
