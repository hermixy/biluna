/*****************************************************************
 * $Id: scan_questionresult.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_QUESTIONRESULT_H
#define SCAN_QUESTIONRESULT_H

#include "rb_objectcontainer.h"

/**
 * Class for question result data
 */
class SCAN_QuestionResult : public RB_ObjectContainer {

public:
    SCAN_QuestionResult(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SCAN_QuestionResult(SCAN_QuestionResult* line);
    virtual ~SCAN_QuestionResult ();

private:
    void createMembers();

};
#endif //SCAN_QUESTIONRESULT_H
