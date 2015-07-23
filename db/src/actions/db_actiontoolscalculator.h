/*****************************************************************
 * $Id: db_actiontoolscalculator.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 25, 2012 1:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSCALCULATOR_H
#define DB_ACTIONTOOLSCALCULATOR_H

#include "rb_action.h"


/**
 * This action class opens the calculator dock window
 */
class DB_EXPORT DB_ActionToolsCalculator : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsCalculator();
    virtual ~DB_ActionToolsCalculator() {}

    static RB_String getName() {
        return "Open calculator";
    }

    virtual RB_String name() {
        return DB_ActionToolsCalculator::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
