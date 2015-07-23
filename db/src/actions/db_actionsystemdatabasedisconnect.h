/*****************************************************************
 * $Id: db_actionsystemdatabasedisconnect.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 2, 2009 7:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMDATABASEDISCONNECT_H
#define DB_ACTIONSYSTEMDATABASEDISCONNECT_H

#include "rb_action.h"

/**
 * Disconnect from all databases, all connections are removed
 */
class DB_EXPORT DB_ActionSystemDatabaseDisconnect : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemDatabaseDisconnect();
    virtual ~DB_ActionSystemDatabaseDisconnect() { }

    static RB_String getName() {
        return "Database Disconnection";
    }

    virtual RB_String name() {
        return DB_ActionSystemDatabaseDisconnect::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();
};

#endif // DB_ACTIONSYSTEMDATABASEDISCONNECT_H
