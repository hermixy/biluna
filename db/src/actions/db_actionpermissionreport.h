/*****************************************************************
 * $Id: db_actionpermissionreport.h 2141 2014-04-07 17:53:21Z rutger $
 * Created: Nov 21, 2015 6:57:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONPERMISSIONREPORT_H
#define DB_ACTIONPERMISSIONREPORT_H

#include "rb_action.h"


/**
 * Action opens allocation report MDI window
 */
class DB_EXPORT DB_ActionPermissionReport : public RB_Action {

    Q_OBJECT

public:
    DB_ActionPermissionReport();
    virtual ~DB_ActionPermissionReport() {}

    static RB_String getName() {
        return "View permission list report";
    }

    virtual RB_String name() {
        return DB_ActionPermissionReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONPERMISSIONREPORT_H
