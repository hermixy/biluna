/*****************************************************************
 * $Id: db_actionsystemclearsettings.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 28, 2011 3:48:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMCLEARSETTINGS_H
#define DB_ACTIONSYSTEMCLEARSETTINGS_H

#include "rb_action.h"

/**
 * This action class clears all settings in the Biluna.ini file
 */
class DB_EXPORT DB_ActionSystemClearSettings : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemClearSettings();
    virtual ~DB_ActionSystemClearSettings() {}

    static RB_String getName() {
        return "Clear Settings";
    }

    virtual RB_String name() {
        return DB_ActionSystemClearSettings::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
