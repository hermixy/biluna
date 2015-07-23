/*****************************************************************
 * $Id: db_actiontoolssimplechart.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 10, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSSIMPLECHART_H
#define DB_ACTIONTOOLSSIMPLECHART_H

#include "rb_action.h"


/**
 * Action opens simple chart window
 */
class DB_EXPORT DB_ActionToolsSimpleChart : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsSimpleChart();
    virtual ~DB_ActionToolsSimpleChart() {}

    static RB_String getName() {
        return "Simple Chart Types";
    }

    virtual RB_String name() {
        return DB_ActionToolsSimpleChart::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONTOOLSSIMPLECHART_H
