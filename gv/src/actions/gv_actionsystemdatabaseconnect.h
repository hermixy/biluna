/*****************************************************************
 * $Id: gv_actionsystemdatabaseconnect.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Apr 19, 2010 3:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSYSTEMDATABASECONNECT_H
#define GV_ACTIONSYSTEMDATABASECONNECT_H

#include "rb_action.h"

/**
 * Connect to database action
 */
class GV_ActionSystemDatabaseConnect : public RB_Action {

    Q_OBJECT

public:
    GV_ActionSystemDatabaseConnect();
    virtual ~GV_ActionSystemDatabaseConnect() { }

    static RB_String getName() {
        return "Database Connection";
    }

    virtual RB_String name() {
        return GV_ActionSystemDatabaseConnect::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // GV_ACTIONSYSTEMDATABASECONNECT_H
