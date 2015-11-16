/*****************************************************************
 * $Id: db_sqlcommonfunctions.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 2, 2013 15:42:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Accounting (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SQLCOMMONFUNCTIONS_H
#define DB_SQLCOMMONFUNCTIONS_H

#include "rb_sqlcommonfunctions.h"

class RB_ObjectContainer;


/**
 * Class with common sql statements
 */
class DB_EXPORT DB_SqlCommonFunctions : public RB_SqlCommonFunctions  {

public:
    DB_SqlCommonFunctions();
    virtual ~DB_SqlCommonFunctions();

    virtual void createDatabaseIndexList(QStringList& indexList);
    virtual void setPerspectiveProjects(RB_ObjectContainer* list);
    virtual void getPermissionList(RB_ObjectContainer* list,
                                   const QString& userName,
                                   const QString& password);
    virtual int getUserCount();

private:
    void appendProjects(RB_ObjectContainer* list, const RB_String& tableName);

};
#endif // DB_SQLCOMMONFUNCTIONS_H
