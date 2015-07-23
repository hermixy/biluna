/*****************************************************************
 * $Id: db_project.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PROJECT_H
#define DB_PROJECT_H

#include "rb_objectcontainer.h"

/**
 * Root class for the DB (Database) model
 * 
 * Note: a project as used for engineering. The class is only for convenience 
 * because the actual data of the user, permission, group and permission to 
 * projects are stored only in the database. This class is for generating the 
 * sql when creating the database and for testing.
 */
class DB_EXPORT DB_Project : public RB_ObjectContainer {

public:
    DB_Project(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_Project(DB_Project* project);
    virtual ~DB_Project();
    
private:
	void createMembers();

};

#endif /*DB_PROJECT_H*/

