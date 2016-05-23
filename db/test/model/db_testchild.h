/*****************************************************************
 * $Id: db_testchild.h 1419 2011-05-16 13:43:55Z rutger $
 * Created: Sep 1, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTCHILD_H
#define DB_TESTCHILD_H

#include "rb_objectcontainer.h"

/**
 * Child class for the DB Test model
 */
class DB_TestChild : public RB_ObjectContainer {

public:
    DB_TestChild(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_TestChild(DB_TestChild* child);
    virtual ~DB_TestChild();

private:
    void createMembers();

};

#endif // DB_TESTCHILD_H
