/*****************************************************************
 * $Id: scan_question.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: Apri, 17 2011 - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the scan or interview (SCAN) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_QUESTION_H
#define SCAN_QUESTION_H

#include "rb_objectcontainer.h"

/**
 * Class for question data
 */
class SCAN_Question : public RB_ObjectContainer {

public:
    SCAN_Question(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SCAN_Question(SCAN_Question* question);
    virtual ~SCAN_Question ();

private:
    void createMembers();

};
#endif //SCAN_QUESTION_H
