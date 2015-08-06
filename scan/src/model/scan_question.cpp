/*****************************************************************
 * $Id: scan_question.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr, 17 2011 - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the scan or interview (SCAN) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_question.h"


/**
 * Constructor
 * @param id Uuid universal unique identification
 * @param p parent object container
 * @param n name of this object, usually SCAN_Question
 * @param f object factory interface, used for cloning and copying of object
 */
SCAN_Question::SCAN_Question (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {

    createMembers();
}


SCAN_Question::SCAN_Question(SCAN_Question* question) : RB_ObjectContainer(question) {
    createMembers();
    *this = *question;
}


SCAN_Question::~SCAN_Question() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * number sequence number of the question
 * type question type such multiple choice all good valued or one good, or open question
 * isactive is question active for this scan yes=1/no=other
 * question text of the question
 */
void SCAN_Question::createMembers() {
    addMember("number", "-", 0, RB2::MemberInteger);
    addMember("type", "-", 0, RB2::MemberInteger);
    addMember("isactive", "-", 1, RB2::MemberInteger);
    addMember("question", "-", "", RB2::MemberString);
}
