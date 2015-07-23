/*****************************************************************
 * $Id: db_actionsystemdatabasebrowser.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 26, 2009 4:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMDATABASEBROWSER_H
#define DB_ACTIONSYSTEMDATABASEBROWSER_H

#include "rb_action.h"

/**
 * Database browser action
 */
class DB_EXPORT DB_ActionSystemDatabaseBrowser : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemDatabaseBrowser();
    virtual ~DB_ActionSystemDatabaseBrowser() { }

    static RB_String getName() {
        return "Database Browser";
    }

    virtual RB_String name() {
        return DB_ActionSystemDatabaseBrowser::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMDATABASEBROWSER_H
