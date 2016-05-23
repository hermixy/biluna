/*****************************************************************
 * $Id: db_testproject.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Sep 1, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTPROJECT_H
#define DB_TESTPROJECT_H

#include "rb_objectcontainer.h"

/**
 * Root class for the DB Test model
 */
class DB_TestProject : public RB_ObjectContainer {

public:
    DB_TestProject(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_TestProject(DB_TestProject* project);
    virtual ~DB_TestProject();

private:
    void createMembers();

};

#endif // DB_TESTPROJECT_H
