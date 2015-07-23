/*****************************************************************
 * $Id: db_actionsystemselectproject.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 8, 2009 21:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMSELECTPROJECT_H
#define DB_ACTIONSYSTEMSELECTPROJECT_H

#include "rb_action.h"

/**
 * Select project from database
 */
class DB_EXPORT DB_ActionSystemSelectProject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemSelectProject();
    virtual ~DB_ActionSystemSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return DB_ActionSystemSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMSELECTPROJECT_H
