/*****************************************************************
 * $Id: acc_actiongltransreport.h 1321 2010-11-10 20:46:28Z rutger $
 * Created: Nov93, 2010 10:57:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONGLTRANSREPORT_H
#define ACC_ACTIONGLTRANSREPORT_H

#include "rb_action.h"


/**
 * Action opens GL transaction report MDI window
 */
class ACC_ActionGlTransReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionGlTransReport();
    virtual ~ACC_ActionGlTransReport() {}

    static RB_String getName() {
        return "View GL transaction report";
    }

    virtual RB_String name() {
        return ACC_ActionGlTransReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONGLTRANSREPORT_H
