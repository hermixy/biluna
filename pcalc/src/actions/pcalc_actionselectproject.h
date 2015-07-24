/*****************************************************************
 * $Id: peng_actionselectproject.h 1366 2011-02-16 21:09:00Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONSELECTPROJECT_H
#define PCALC_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select project from database
 */
class PCALC_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionSelectProject();
    virtual ~PCALC_ActionSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return PCALC_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONSELECTPROJECT_H
