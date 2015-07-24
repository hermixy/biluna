/*****************************************************************
 * $Id: peng_project.h 1366 2011-02-16 21:09:00Z rutger $
 * Created: Apr 15, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PROJECT_H
#define PENG_PROJECT_H

#include "rb_objectcontainer.h"

/**
 * Root class for the PENG model
 */
class PENG_Project : public RB_ObjectContainer {

public:
    PENG_Project(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Project(PENG_Project* project);
    virtual ~PENG_Project();

private:
    void createMembers();

};

#endif // PENG_PROJECT_H
