/*****************************************************************
 * $Id: db_actioneditselectall.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 8, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITSELECTALL_H
#define DB_ACTIONEDITSELECTALL_H

#include "rb_action.h"

/**
 * This action class can handle user events for selecting all data
 */
class DB_EXPORT DB_ActionEditSelectAll : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditSelectAll();
    virtual ~DB_ActionEditSelectAll() {}

    static RB_String getName() {
        return "Edit Select All";
    }

    virtual RB_String name() {
        return DB_ActionEditSelectAll::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
