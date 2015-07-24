/*****************************************************************
 * $Id: sail_project.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_PROJECT_H
#define SAIL_PROJECT_H

#include "rb_objectcontainer.h"

/**
 * Class for SAIL project which is the root object
 */
class SAIL_Project : public RB_ObjectContainer {

public:
    SAIL_Project(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SAIL_Project(SAIL_Project* obj);
    virtual ~SAIL_Project();

private:
    void createMembers();

};

#endif // SAIL_PROJECT_H
