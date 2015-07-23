/*****************************************************************
 * $Id: db_actiontoolssimplereport.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sept 27, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSSIMPLEREPORT_H
#define DB_ACTIONTOOLSSIMPLEREPORT_H

#include "rb_action.h"


/**
 * This action class opens the window to create a simple reports
 * from the database
 */
class DB_EXPORT DB_ActionToolsSimpleReport : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsSimpleReport();
    virtual ~DB_ActionToolsSimpleReport() {}

    static RB_String getName() {
        return "Create simple report";
    }

    virtual RB_String name() {
        return DB_ActionToolsSimpleReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
