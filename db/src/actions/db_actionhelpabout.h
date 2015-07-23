/*****************************************************************
 * $Id: db_actionhelpabout.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 26, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONHELPABOUT_H
#define DB_ACTIONHELPABOUT_H

#include "rb_action.h"


/**
 * This action class can handle user events to open files.
 */
class DB_EXPORT DB_ActionHelpAbout : public RB_Action {

    Q_OBJECT

public:
    DB_ActionHelpAbout();
    virtual ~DB_ActionHelpAbout() {}

    static RB_String getName() {
        return "Help About";
    }

    virtual RB_String name() {
        return DB_ActionHelpAbout::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
