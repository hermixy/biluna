/*****************************************************************
 * $Id: db_actionsystemversion.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 2, 2013 1:57:42 AM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMVERSION_H
#define DB_ACTIONSYSTEMVERSION_H

#include "rb_action.h"

/**
 * View perspective database table versions
 */
class DB_EXPORT DB_ActionSystemVersion : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemVersion();
    virtual ~DB_ActionSystemVersion() { }

    static RB_String getName() {
        return "View database version";
    }

    virtual RB_String name() {
        return DB_ActionSystemVersion::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMVERSION_H
