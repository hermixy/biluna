/*****************************************************************
 * $Id: db_actionformatremove.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATREMOVE_H
#define DB_ACTIONFORMATREMOVE_H

#include "rb_action.h"

/**
 * This action class can handle user events for removing text formatting
 */
class DB_EXPORT DB_ActionFormatRemove : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatRemove();
    virtual ~DB_ActionFormatRemove() {}

    static RB_String getName() {
        return "Format Remove";
    }

    virtual RB_String name() {
        return DB_ActionFormatRemove::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
