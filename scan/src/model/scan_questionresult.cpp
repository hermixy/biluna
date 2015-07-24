/*****************************************************************
 * $Id: scan_questionresult.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_questionresult.h"


/**
 * Constructor
 * @param id Uuid universal unique identification
 * @param p parent object container
 * @param n name of this object, usually SCAN_QuestionResult
 * @param f object factory interface, used for cloning and copying of object
 */
SCAN_QuestionResult::SCAN_QuestionResult (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectContainer (id, p, n, f) {

    createMembers();
}


SCAN_QuestionResult::SCAN_QuestionResult(SCAN_QuestionResult* question)
                        : RB_ObjectContainer(question) {
    createMembers();
    *this = *question;
}


SCAN_QuestionResult::~SCAN_QuestionResult() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}

/**
 * Create members:
 * number sequence number of the question
 * type question type such multiple choice all good valued or one good, or open question
 * question text of the question
 */
void SCAN_QuestionResult::createMembers() {
    addMember(this, "number", "-", 0, RB2::MemberInteger);
    addMember(this, "type", "-", 0, RB2::MemberInteger);
    addMember(this, "question", "", "", RB2::MemberString);
}
