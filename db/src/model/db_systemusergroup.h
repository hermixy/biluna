/*****************************************************************
 * $Id: db_systemusergroup.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2006 - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SYSTEMUSERGROUP_H
#define DB_SYSTEMUSERGROUP_H

#include "rb_objectatomic.h"

/**
 * Group of which the user is a member. A user can be member
 * of more than one group
 */
class DB_EXPORT DB_SystemUserGroup : public RB_ObjectAtomic {

public:
    DB_SystemUserGroup(const RB_String& id = "", RB_ObjectBase *p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_SystemUserGroup(DB_SystemUserGroup* group);
    virtual ~DB_SystemUserGroup();
    
private:
	void createMembers();

};

#endif /*DB_SYSTEMUSERGROUP_H*/
