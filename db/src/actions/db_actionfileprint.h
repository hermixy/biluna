/*****************************************************************
 * $Id: db_actionfileprint.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILEPRINT_H
#define DB_ACTIONFILEPRINT_H

#include "rb_action.h"

class RB_Dialog;

/**
 * This action class can handle user events to print the current file.
 */
class DB_EXPORT DB_ActionFilePrint : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFilePrint();
    virtual ~DB_ActionFilePrint() { }

    static RB_String getName() {
        return "File Print";
    }

    virtual RB_String name() {
        return DB_ActionFilePrint::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
    virtual void printDialog(RB_Dialog* dialog);
};

#endif
