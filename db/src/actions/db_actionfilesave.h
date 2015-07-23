/*****************************************************************
 * $Id: db_actionfilesave.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILESAVE_H
#define DB_ACTIONFILESAVE_H

#include "rb_action.h"


/**
 * This action class can handle user events to save the current file.
 */
class DB_EXPORT DB_ActionFileSave : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileSave();
    virtual ~DB_ActionFileSave() { }

    static RB_String getName() {
        return "File Save";
    }

    virtual RB_String name() {
        return DB_ActionFileSave::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
