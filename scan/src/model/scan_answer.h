/*****************************************************************
 * $Id: scan_answer.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ANSWER_H
#define SCAN_ANSWER_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the possible answers to the parent question,
 * in case of multiple choice questions. This class holds otherwise
 * the an example good answer or expected words to an open question.
 */
class SCAN_Answer : public RB_ObjectContainer {

public:
    SCAN_Answer(const RB_String& id = "", RB_ObjectBase* p = 0,
                        const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SCAN_Answer(SCAN_Answer* answer);
    virtual ~SCAN_Answer ();

private:
    void createMembers();

};

#endif /*SCAN_ANSWER_H*/
