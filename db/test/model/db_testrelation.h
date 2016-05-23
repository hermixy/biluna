/*****************************************************************
 * $Id: db_testrelation.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Sep 2, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTRELATION_H
#define DB_TESTRELATION_H

#include "rb_objectatomic.h"

/**
 * Relational class to base test class of the DB Test model
 */
class DB_TestRelation : public RB_ObjectAtomic {

public:
    DB_TestRelation(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_TestRelation(DB_TestRelation* relation);
    virtual ~DB_TestRelation();

private:
    void createMembers();

};

#endif // DB_TESTRELATION_H
