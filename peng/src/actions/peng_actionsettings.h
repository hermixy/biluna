/*****************************************************************
 * $Id: peng_actionsettings.h 2155 2014-08-04 16:50:28Z rutger $
 * Created: Aug 4, 2014 12:57:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONSETTINGS_H
#define PENG_ACTIONSETTINGS_H

#include "rb_action.h"

/**
 * Edit main settings
 */
class PENG_ActionSettings : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionSettings();
    virtual ~PENG_ActionSettings() { }

    static RB_String getName() {
        return "Edit main settings";
    }

    virtual RB_String name() {
        return PENG_ActionSettings::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONSETTINGS_H
