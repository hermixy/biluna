/*****************************************************************
 * $Id: db_actionsystempermission.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 2, 2013 10:14:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMPERMISSION_H
#define DB_ACTIONSYSTEMPERMISSION_H

#include "rb_action.h"


/**
 * Action for editing permission dialog
 */
class DB_EXPORT DB_ActionSystemPermission : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemPermission();
    virtual ~DB_ActionSystemPermission() {}

    static RB_String getName() {
        return "User permissions";
    }

    virtual RB_String name() {
        return DB_ActionSystemPermission::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMPERMISSION_H
