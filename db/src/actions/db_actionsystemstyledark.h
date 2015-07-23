/*****************************************************************
 * $Id: db_actionsystemstyledark.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 29, 2014 1:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMSTYLEDARK_H
#define DB_ACTIONSYSTEMSTYLEDARK_H

#include "rb_action.h"


/**
 * System action class for setting a dark style to the GUI
 */
class DB_EXPORT DB_ActionSystemStyleDark : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemStyleDark();
    virtual ~DB_ActionSystemStyleDark() {}

    static RB_String getName() {
        return "Dark Style";
    }

    virtual RB_String name() {
        return DB_ActionSystemStyleDark::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
