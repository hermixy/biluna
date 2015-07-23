/*****************************************************************
 * $Id: db_actionfilequit.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILEQUIT_H
#define DB_ACTIONFILEQUIT_H

#include "rb_action.h"


/**
 * This action class can handle user events to quit the application.
 */
class DB_EXPORT DB_ActionFileQuit : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileQuit();
    virtual ~DB_ActionFileQuit() { }

    static RB_String getName() {
        return "File Quit";
    }

    virtual RB_String name() {
        return DB_ActionFileQuit::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
