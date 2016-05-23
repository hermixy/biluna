/*****************************************************************
 * $Id: db_testrelation.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Sep 2, 2009 9:04:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testrelation.h"


DB_TestRelation::DB_TestRelation (const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic (id, p, n, f) {
    createMembers();
}


DB_TestRelation::DB_TestRelation(DB_TestRelation* relation)
        : RB_ObjectAtomic(relation) {
    createMembers();
    *this = *relation;
}


DB_TestRelation::~DB_TestRelation() {
    // clean up children done in RB_ObjectBase and RB_ObjectContainer
}


void DB_TestRelation::createMembers() {
    addMember("relation", "-", "New Relation", RB2::MemberChar125);
}
