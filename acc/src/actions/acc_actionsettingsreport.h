/*****************************************************************
 * $Id: acc_actionsettingsreport.h 1481 2011-11-21 19:28:26Z rutger $
 * Created: Nov 20, 2011 11:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSETTINGSREPORT_H
#define ACC_ACTIONSETTINGSREPORT_H

#include "rb_action.h"


/**
 * Action opens settings report
 */
class ACC_ActionSettingsReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSettingsReport();
    virtual ~ACC_ActionSettingsReport() {}

    static RB_String getName() {
        return "Settings report";
    }

    virtual RB_String name() {
        return ACC_ActionSettingsReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSETTINGSREPORT_H
