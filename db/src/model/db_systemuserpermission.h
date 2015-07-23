/*****************************************************************
 * $Id: db_systemuserpermission.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SYSTEMUSERPERMISSION_H
#define DB_SYSTEMUSERPERMISSION_H

#include "rb_objectatomic.h"

/**
 * User permission of Biluna database.
 * Internal object class only to store the permissions of the user
 * not part of the database models
 */
class DB_EXPORT DB_SystemUserPermission : public RB_ObjectAtomic {

public:
    DB_SystemUserPermission(const RB_String& id = "", RB_ObjectBase *p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_SystemUserPermission(DB_SystemUserPermission* userPermission);
    virtual ~DB_SystemUserPermission();
    
private:
	void createMembers();

};

#endif /*DB_SYSTEMUSERPERMISSION_H*/
