/*****************************************************************
 * $Id: db_actionsystemgroup.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 18, 2014 12:26:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMGROUP_H
#define DB_ACTIONSYSTEMGROUP_H

#include "rb_action.h"

/**
 * Edit system group. The system users can be part of a system group.
 * In turn, authorizations/premission are assigned to a system group.
 */
class DB_EXPORT DB_ActionSystemGroup : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemGroup();
    virtual ~DB_ActionSystemGroup() { }

    static RB_String getName() {
        return "Edit system group";
    }

    virtual RB_String name() {
        return DB_ActionSystemGroup::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMGROUP_H
