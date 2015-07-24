/*****************************************************************
 * $Id: srm_actionactivity.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 20, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONACTIVITY_H
#define SRM_ACTIONACTIVITY_H

#include "rb_action.h"


/**
 * Action opens activity edit window
 */
class SRM_ActionActivity : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionActivity();
    virtual ~SRM_ActionActivity() {}

    static RB_String getName() {
        return "Activity";
    }

    virtual RB_String name() {
        return SRM_ActionActivity::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONACTIVITY_H
