/*****************************************************************
 * $Id: db_actionsystemeditproject.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2012 9:57:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMEDITPROJECT_H
#define DB_ACTIONSYSTEMEDITPROJECT_H

#include "rb_action.h"

/**
 * Edit projects
 */
class DB_EXPORT DB_ActionSystemEditProject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemEditProject();
    virtual ~DB_ActionSystemEditProject() { }

    static RB_String getName() {
        return "Edit project";
    }

    virtual RB_String name() {
        return DB_ActionSystemEditProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMEDITPROJECT_H
