/*****************************************************************
 * $Id: db_actionhelpsubject.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 3, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONHELPSUBJECT_H
#define DB_ACTIONHELPSUBJECT_H

#include "rb_action.h"

/**
 * This action class opens a help subject
 */
class DB_EXPORT DB_ActionHelpSubject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionHelpSubject();
    virtual ~DB_ActionHelpSubject() { }

    static RB_String getName() {
        return "Help Subject";
    }

    virtual RB_String name() {
        return DB_ActionHelpSubject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();
};

#endif // DB_ACTIONHELPSUBJECT_H

