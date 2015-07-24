/*****************************************************************
 * $Id: crm_actionactivity.h 1553 2012-02-20 21:46:03Z rutger $
 * Created: Feb 20, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONACTIVITY_H
#define CRM_ACTIONACTIVITY_H

#include "rb_action.h"


/**
 * Action opens activity edit window
 */
class CRM_ActionActivity : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionActivity();
    virtual ~CRM_ActionActivity() {}

    static RB_String getName() {
        return "Activity";
    }

    virtual RB_String name() {
        return CRM_ActionActivity::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONACTIVITY_H
