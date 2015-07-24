/*****************************************************************
 * $Id: sail_actionselectproject.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ACTIONSELECTPROJECT_H
#define SAIL_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select root project from database
 */
class SAIL_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    SAIL_ActionSelectProject();
    virtual ~SAIL_ActionSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return SAIL_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SAIL_ACTIONSELECTPROJECT_H
