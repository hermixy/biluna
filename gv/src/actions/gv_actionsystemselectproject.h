/*****************************************************************
 * $Id: gv_actionsystemselectproject.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Apr 19, 2010 21:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSYSTEMSELECTPROJECT_H
#define GV_ACTIONSYSTEMSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select project from database
 */
class GV_ActionSystemSelectProject : public RB_Action {

    Q_OBJECT

public:
    GV_ActionSystemSelectProject();
    virtual ~GV_ActionSystemSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return GV_ActionSystemSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // GV_ACTIONSYSTEMSELECTPROJECT_H
