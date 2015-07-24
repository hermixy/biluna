/*****************************************************************
 * $Id: scan_actionselectproject.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: Apr 26, 2011 21:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ACTIONSELECTPROJECT_H
#define SCAN_ACTIONSELECTPROJECT_H

#include "rb_action.h"


/**
 * Select project from database
 */
class SCAN_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    SCAN_ActionSelectProject();
    virtual ~SCAN_ActionSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return SCAN_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SCAN_ACTIONSELECTPROJECT_H
