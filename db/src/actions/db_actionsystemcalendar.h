/*****************************************************************
 * $Id: db_actionsystemcalendar.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Apr 22, 2013 4:14:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMCALENDAR_H
#define DB_ACTIONSYSTEMCALENDAR_H

#include "rb_action.h"


/**
 * Action opens calendar window
 */
class DB_EXPORT DB_ActionSystemCalendar : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemCalendar();
    virtual ~DB_ActionSystemCalendar() {}

    static RB_String getName() {
        return "Calendar";
    }

    virtual RB_String name() {
        return DB_ActionSystemCalendar::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMCALENDAR_H
