/*****************************************************************
 * $Id: db_actiontestdbobject.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Dec 15, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTDBOBJECT_H
#define DB_ACTIONTESTDBOBJECT_H

#include "rb_action.h"


/**
 * Test action class for data object of Biluna.
 */
class DB_ActionTestDbObject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestDbObject();
    virtual ~DB_ActionTestDbObject() {}

    static RB_String getName() {
        return "Test DB object";
    }

    virtual RB_String name() {
        return DB_ActionTestDbObject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

private:

};

#endif // DB_ACTIONTESTDBOBJECT_H
