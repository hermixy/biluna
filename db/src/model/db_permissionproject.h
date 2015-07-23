/*****************************************************************
 * $Id: db_permissionproject.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2007 - rutger
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna AUTH project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONPROJECT_H
#define DB_PERMISSIONPROJECT_H

#include "rb_objectcontainer.h"

/**
 * Class defining the access authorization of a group to the DB (Database)
 * The authorization includes the applicable part of the database such as access
 * to projects, disciplines and plugin/modules.
 */
class DB_EXPORT DB_PermissionProject : public RB_ObjectContainer {

public:
    DB_PermissionProject(const RB_String& id = "", RB_ObjectBase *p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_PermissionProject(DB_PermissionProject* projectRoot);
    virtual ~DB_PermissionProject();
    
private:
	void createMembers();

};

#endif /*DB_PERMISSIONPROJECT_H*/
