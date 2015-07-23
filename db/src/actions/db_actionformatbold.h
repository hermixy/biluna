/*****************************************************************
 * $Id: db_actionformatbold.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 9, 2010 5:48:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATBOLD_H
#define DB_ACTIONFORMATBOLD_H

#include "rb_action.h"

/**
 * This action class can handle user events for formating bold text
 */
class DB_EXPORT DB_ActionFormatBold : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatBold();
    virtual ~DB_ActionFormatBold() {}

    static RB_String getName() {
        return "Format Bold";
    }

    virtual RB_String name() {
        return DB_ActionFormatBold::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
