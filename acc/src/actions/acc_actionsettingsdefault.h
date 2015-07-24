/*****************************************************************
 * $Id: acc_actionsettingsdefault.h 2113 2014-02-24 16:56:52Z rutger $
 * Created: Feb 24, 2014 15:45:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSETTINGSDEFAULT_H
#define ACC_ACTIONSETTINGSDEFAULT_H

#include "rb_action.h"


/**
 * Action class for to use first defaults for settings,
 * such as the ACC_ChartMaster.
 */
class ACC_ActionSettingsDefault : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSettingsDefault();
    virtual ~ACC_ActionSettingsDefault() {}

    static RB_String getName() {
        return "Default settings";
    }

    virtual RB_String name() {
        return ACC_ActionSettingsDefault::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
