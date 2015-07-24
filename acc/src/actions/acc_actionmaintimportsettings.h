/*****************************************************************
 * $Id: acc_actionmaintimportsettings.h 1518 2012-01-16 19:36:49Z rutger $
 * Created: Jan 16, 2012 19:54:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONMAINTIMPORTSETTINGS_H
#define ACC_ACTIONMAINTIMPORTSETTINGS_H

#include "rb_action.h"


/**
 * Maintenance action class for the import of settings.
 */
class ACC_ActionMaintImportSettings : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionMaintImportSettings();
    virtual ~ACC_ActionMaintImportSettings() {}

    static RB_String getName() {
        return "Import settings";
    }

    virtual RB_String name() {
        return ACC_ActionMaintImportSettings::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
