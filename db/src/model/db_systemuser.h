/*****************************************************************
 * $Id: db_systemuser.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2006 - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SYSTEMUSER_H
#define DB_SYSTEMUSER_H

#include "rb_objectcontainer.h"

/**
 * User of Biluna database.
 * The function of this class is only used when the user is connected
 * to a database. Actual data of the user, permission, group and permission
 * to projects are stored in the database.
 *
 * TODO: This class can also be used for the employees in a company,
 * next to the ACC_Contact class
 *
 * Note: db_user gave problems with MySQL database
 */
class DB_EXPORT DB_SystemUser : public RB_ObjectContainer {

public:
    DB_SystemUser(const RB_String& id = "", RB_ObjectBase *p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_SystemUser(DB_SystemUser* user);
    virtual ~DB_SystemUser();
    
private:
	void createMembers();

};

#endif /*DB_SYSTEMUSER_H*/
