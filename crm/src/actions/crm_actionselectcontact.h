/*****************************************************************
 * $Id: crm_actionselectcontact.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONSELECTPROJECT_H
#define CRM_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select project from database
 */
class CRM_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionSelectProject();
    virtual ~CRM_ActionSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return CRM_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONSELECTPROJECT_H
