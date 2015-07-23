/*****************************************************************
 * $Id: db_actionformatalignleft.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATALIGNLEFT_H
#define DB_ACTIONFORMATALIGNLEFT_H

#include "rb_action.h"

/**
 * This action class can handle user events for left aligning text
 */
class DB_EXPORT DB_ActionFormatAlignLeft : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatAlignLeft();
    virtual ~DB_ActionFormatAlignLeft() {}

    static RB_String getName() {
        return "Format Align Left";
    }

    virtual RB_String name() {
        return DB_ActionFormatAlignLeft::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
