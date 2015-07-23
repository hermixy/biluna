/*****************************************************************
 * $Id: db_actionsystemtoolsviewmodel.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 24, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSVIEWMODEL_H
#define DB_ACTIONSYSTEMTOOLSVIEWMODEL_H

#include "rb_action.h"


/**
 * Tools action class to view the model in a text window.
 */
class DB_EXPORT DB_ActionSystemToolsViewModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsViewModel();
    virtual ~DB_ActionSystemToolsViewModel() {}

    static RB_String getName() {
        return "View Model";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsViewModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
