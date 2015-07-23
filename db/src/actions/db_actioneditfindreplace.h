/*****************************************************************
 * $Id: db_actioneditfindreplace.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 22, 2012 10:34:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITFINDREPLACE_H
#define DB_ACTIONEDITFINDREPLACE_H

#include "rb_action.h"

/**
 * Find and replace in text action
 */
class DB_EXPORT DB_ActionEditFindReplace : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditFindReplace();
    virtual ~DB_ActionEditFindReplace() { }

    static RB_String getName() {
        return "Find and Replace";
    }

    virtual RB_String name() {
        return DB_ActionEditFindReplace::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONEDITFINDREPLACE_H
