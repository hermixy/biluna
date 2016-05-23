/*****************************************************************
 * $Id: db_actiontestdatabasemodel.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTDATABASEMODEL_H
#define DB_ACTIONTESTDATABASEMODEL_H

#include "rb_action.h"


/**
 * Test action class database table model
 */
class DB_ActionTestDatabaseModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestDatabaseModel();
    virtual ~DB_ActionTestDatabaseModel() {}

    static RB_String getName() {
        return "Test Database Model";
    }

    virtual RB_String name() {
        return DB_ActionTestDatabaseModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
