/*****************************************************************
 * $Id: db_actioneditfind.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 22, 2012 10:34:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITFIND_H
#define DB_ACTIONEDITFIND_H

#include "rb_action.h"

/**
 * Find in text action
 */
class DB_EXPORT DB_ActionEditFind : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditFind();
    virtual ~DB_ActionEditFind() { }

    static RB_String getName() {
        return "Find";
    }

    virtual RB_String name() {
        return DB_ActionEditFind::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONEDITFIND_H
