/*****************************************************************
 * $Id: db_actionsystemdatabasemapping.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 26, 2009 3:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMDATABASEMAPPING_H
#define DB_ACTIONSYSTEMDATABASEMAPPING_H

#include "rb_action.h"

/**
 * Connect to database action
 */
class DB_EXPORT DB_ActionSystemDatabaseMapping : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemDatabaseMapping();
    virtual ~DB_ActionSystemDatabaseMapping() { }

    static RB_String getName() {
        return "Database Mapping";
    }

    virtual RB_String name() {
        return DB_ActionSystemDatabaseMapping::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMDATABASEMAPPING_H
