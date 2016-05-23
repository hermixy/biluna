/*****************************************************************
 * $Id: db_actiontestrelation.h 1279 2010-08-11 19:11:45Z rutger $
 * Created: Aug 9, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTESTRELATION_H
#define DB_ACTIONTESTRELATION_H

#include "rb_action.h"


/**
 * Test action class relation database table model
 */
class DB_ActionTestRelation : public RB_Action {

    Q_OBJECT

public:
    DB_ActionTestRelation();
    virtual ~DB_ActionTestRelation() {}

    static RB_String getName() {
        return "Test Relation Database Model";
    }

    virtual RB_String name() {
        return DB_ActionTestRelation::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONTESTRELATION_H
