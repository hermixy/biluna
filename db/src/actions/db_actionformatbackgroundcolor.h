/*****************************************************************
 * $Id: db_actionformatbackgroundcolor.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATBACKGROUNDCOLOR_H
#define DB_ACTIONFORMATBACKGROUNDCOLOR_H

#include "rb_action.h"

/**
 * This action class can handle user events for selecting all data
 */
class DB_EXPORT DB_ActionFormatBackgroundColor : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatBackgroundColor();
    virtual ~DB_ActionFormatBackgroundColor() {}

    static RB_String getName() {
        return "Format Background Color";
    }

    virtual RB_String name() {
        return DB_ActionFormatBackgroundColor::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
