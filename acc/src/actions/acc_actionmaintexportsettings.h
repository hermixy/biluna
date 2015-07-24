/*****************************************************************
 * $Id: acc_actionmaintexportsettings.h 1518 2012-01-16 19:36:49Z rutger $
 * Created: Jan 16, 2012 11:54:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONMAINTEXPORTSETTINGS_H
#define ACC_ACTIONMAINTEXPORTSETTINGS_H

#include "rb_action.h"


/**
 * Maintenance action class for the export of settings.
 */
class ACC_ActionMaintExportSettings : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionMaintExportSettings();
    virtual ~ACC_ActionMaintExportSettings() {}

    static RB_String getName() {
        return "Export Settings";
    }

    virtual RB_String name() {
        return ACC_ActionMaintExportSettings::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
