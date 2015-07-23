/*****************************************************************
 * $Id: db_actionsystemdatabaseconnect.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 14, 2009 3:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMDATABASECONNECT_H
#define DB_ACTIONSYSTEMDATABASECONNECT_H

#include "rb_action.h"

/**
 * Connect to database action
 */
class DB_EXPORT DB_ActionSystemDatabaseConnect : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemDatabaseConnect();
    virtual ~DB_ActionSystemDatabaseConnect() { }

    static RB_String getName() {
        return "Database Connection";
    }

    virtual RB_String name() {
        return DB_ActionSystemDatabaseConnect::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMDATABASECONNECT_H
