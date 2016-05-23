/*****************************************************************
 * $Id: db_actionsystemselectproject.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Aug 6, 2010 21:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTSYSTEMSELECTPROJECT_H
#define DB_ACTIONTESTSYSTEMSELECTPROJECT_H

#include "rb_action.h"

/**
 * Select test project from database
 */
class DB_ActionTestSystemSelectProject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestSystemSelectProject();
    virtual ~DB_ActionTestSystemSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return DB_ActionTestSystemSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONTESTSYSTEMSELECTPROJECT_H
