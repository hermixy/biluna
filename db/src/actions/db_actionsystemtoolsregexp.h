/*****************************************************************
 * $Id: db_actionsystemtoolsregexp.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 22, 2012 7:38:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSREGEXP_H
#define DB_ACTIONSYSTEMTOOLSREGEXP_H

#include "rb_action.h"

/**
 * Regular expression test action
 */
class DB_EXPORT DB_ActionSystemToolsRegExp : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsRegExp();
    virtual ~DB_ActionSystemToolsRegExp() { }

    static RB_String getName() {
        return "Regular expression test dialog";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsRegExp::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMTOOLSREGEXP_H
