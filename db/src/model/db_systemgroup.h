/*****************************************************************
 * $Id: db_systemgroup.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2014-04-24 - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SYSTEMGROUP_H
#define DB_SYSTEMGROUP_H

#include "rb_objectcontainer.h"

/**
 * Class defining a group. Users will be assigned to a group. A group
 * will be assigned permissions in a permission project.
 *
 * Note: db_group gave problems with MySQL database
 */
class DB_EXPORT DB_SystemGroup : public RB_ObjectContainer {

public:
    DB_SystemGroup(const RB_String& id = "", RB_ObjectBase *p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_SystemGroup(DB_SystemGroup* projectRoot);
    virtual ~DB_SystemGroup();
    
private:
	void createMembers();

};

#endif /*DB_SYSTEMGROUP_H*/
