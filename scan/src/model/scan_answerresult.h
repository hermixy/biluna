/*****************************************************************
 * $Id: scan_answerresult.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ANSWERRESULT_H
#define SCAN_ANSWERRESULT_H

#include "rb_objectcontainer.h"


/**
 * Class that holds actual answer by the person
 * doing the interview or scan.
 */
class SCAN_AnswerResult : public RB_ObjectContainer {

public:
    SCAN_AnswerResult(const RB_String& id = "", RB_ObjectBase* p = 0,
                        const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SCAN_AnswerResult(SCAN_AnswerResult* answerResult);
    virtual ~SCAN_AnswerResult ();

private:
    void createMembers();

};

#endif /*SCAN_ANSWERRESULT_H*/
