/*****************************************************************
 * $Id: db_actionformataligncenter.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATALIGNCENTER_H
#define DB_ACTIONFORMATALIGNCENTER_H


#include "rb_action.h"

/**
 * This action class can handle user events for aligning text centered
 */
class DB_EXPORT DB_ActionFormatAlignCenter : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatAlignCenter();
    virtual ~DB_ActionFormatAlignCenter() {}

    static RB_String getName() {
        return "Formate Align Center";
    }

    virtual RB_String name() {
        return DB_ActionFormatAlignCenter::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
