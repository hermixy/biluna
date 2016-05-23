/*****************************************************************
 * $Id: db_test.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Sep 2, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TEST_H
#define DB_TEST_H

#include "rb_objectcontainer.h"

/**
 * Base class for the DB Test model
 */
class DB_Test : public RB_ObjectContainer {

public:
    DB_Test(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_Test(DB_Test* test);
    virtual ~DB_Test();

private:
    void createMembers();

};

#endif // DB_TEST_H
