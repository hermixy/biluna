/*****************************************************************
 * $Id: db_actiontestdialogmodel.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTDIALOGMODEL_H
#define DB_ACTIONTESTDIALOGMODEL_H

#include "rb_action.h"

/**
 * Action to test the dialog action
 */
class DB_ActionTestDialogModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestDialogModel();
    virtual ~DB_ActionTestDialogModel() { }

    static RB_String getName() {
        return "Test Dialog Model";
    }

    virtual RB_String name() {
        return DB_ActionTestDialogModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONTESTDIALOGMODEL_H
