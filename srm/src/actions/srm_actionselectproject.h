/*****************************************************************
 * $Id: srm_actionselectproject.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONSELECTPROJECT_H
#define SRM_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select project or CRM account from database
 */
class SRM_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionSelectProject();
    virtual ~SRM_ActionSelectProject() { }

    static RB_String getName() {
        return "Select account";
    }

    virtual RB_String name() {
        return SRM_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONSELECTPROJECT_H
