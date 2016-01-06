/*****************************************************************
 * $Id: std_project.h 0001 2016-01-06T20:31:37 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_PROJECT_H
#define STD_PROJECT_H

#include "rb_objectatomic.h"

/**
 * Root object for standards database
 */
class DB_EXPORT STD_Project : public RB_ObjectAtomic {

public:
    STD_Project(const QString& id = "", RB_ObjectBase* p = NULL,
               const QString& n = , RB_ObjectFactory* f = NULL);
    STD_Project(STD_Project* obj);
    virtual ~STD_Project();

private:
    void createMembers();

};

#endif /*STD_PROJECT_H*/
