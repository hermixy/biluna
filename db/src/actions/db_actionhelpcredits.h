/*****************************************************************
 * $Id: db_actionhelpcredits.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 22, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONHELPCREDITS_H
#define DB_ACTIONHELPCREDITS_H

#include "rb_action.h"


/**
 * This action class can handle user events to open files.
 */
class DB_EXPORT DB_ActionHelpCredits : public RB_Action {

    Q_OBJECT

public:
    DB_ActionHelpCredits();
    virtual ~DB_ActionHelpCredits() {}

    static RB_String getName() {
        return "Help Credits";
    }

    virtual RB_String name() {
        return DB_ActionHelpCredits::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
