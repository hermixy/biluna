/*****************************************************************
 * $Id: scan_answerresult.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_answerresult.h"

SCAN_AnswerResult::SCAN_AnswerResult (const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
                        : RB_ObjectContainer (id, p, n, f) {
    createMembers();
}


SCAN_AnswerResult::SCAN_AnswerResult(SCAN_AnswerResult* answerResult)
                                    : RB_ObjectContainer(answerResult) {
    createMembers();
    *this = *answerResult;
}

SCAN_AnswerResult::~SCAN_AnswerResult () {
    // deletion of children and members is done in RB_ObjectBase
    // and RB_ObjectContainer
}

/**
 * Create member:
 * number sequence number of the answer
 * answer in case of multiple choice is repeat of possible answer,
 *   in case of open quesion this is the answer given by the person
 *   being the scanned or interviewed
 * mvalue in case of multiple choice all valued it is the the value given,
 *   in case of multiple choice one good 1 means selected
 *   other values not selected, in case of open question this value has
 *   no meaning.
 */
void SCAN_AnswerResult::createMembers() {
    addMember("number", "-", 1, RB2::MemberInteger);
    addMember("answer", "-", "", RB2::MemberString);
    addMember("mvalue", "-", "", RB2::MemberInteger);
}
