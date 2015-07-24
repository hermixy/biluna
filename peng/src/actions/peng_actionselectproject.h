/*****************************************************************
 * $Id: peng_actionselectproject.h 1366 2011-02-16 21:09:00Z rutger $
 * Created: Feb 16, 2011 21:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONSELECTPROJECT_H
#define PENG_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select project from database
 */
class PENG_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionSelectProject();
    virtual ~PENG_ActionSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return PENG_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONSELECTPROJECT_H
