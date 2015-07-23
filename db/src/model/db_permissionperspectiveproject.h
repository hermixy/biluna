/*****************************************************************
 * $Id: db_permissionperspectiveproject.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2015-01-04
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONPERSPECTIVEPROJECT_H
#define DB_PERMISSIONPERSPECTIVEPROJECT_H

#include "rb_objectcontainer.h"

/**
 * Permission perspective project, is a perspective project assigned
 * to a permission project. The assigned groups will have the relevant
 * permissions to the perspective project
 */
class DB_EXPORT DB_PermissionPerspectiveProject : public RB_ObjectContainer {

public:
    DB_PermissionPerspectiveProject(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_PermissionPerspectiveProject(DB_PermissionPerspectiveProject* project);
    virtual ~DB_PermissionPerspectiveProject();
    
private:
	void createMembers();

};

#endif /*DB_PERMISSIONPERSPECTIVEPROJECT_H*/

