/*****************************************************************
 * $Id: db_version.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sept 2, 2013 10:00:42 AM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_VERSION_H
#define DB_VERSION_H

#include "rb_objectatomic.h"

/**
 * Version of database per perspective. The version of the database
 * is checked when the perspective is loaded in XXX_ActionSelectProject.
 * The database tables will be updated if the database is not up to date.
 */
class DB_EXPORT DB_Version : public RB_ObjectAtomic {

public:
    DB_Version(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_Version(DB_Version* version);
    virtual ~DB_Version();
    
private:
	void createMembers();

};

#endif /*DB_VERSION_H*/
