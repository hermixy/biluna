/*****************************************************************
 * $Id: db_permissiongroup.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2007 - rutger
 *
 * Copyright (C) 2007 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONGROUP_H
#define DB_PERMISSIONGROUP_H

#include "rb_objectatomic.h"

/**
 * Group of users relevant to the permission project
 */
class DB_EXPORT DB_PermissionGroup : public RB_ObjectAtomic {

public:
    DB_PermissionGroup(const RB_String& id = "", RB_ObjectBase *p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_PermissionGroup(DB_PermissionGroup* group);
    virtual ~DB_PermissionGroup();
    
private:
	void createMembers();

};

#endif /*DB_PERMISSIONGROUP_H*/
