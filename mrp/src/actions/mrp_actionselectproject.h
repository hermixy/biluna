/*****************************************************************
 * $Id: sail_actionselectproject.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_ACTIONSELECTPROJECT_H
#define MRP_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select root project from database
 */
class MRP_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    MRP_ActionSelectProject();
    virtual ~MRP_ActionSelectProject() { }

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return MRP_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // MRP_ACTIONSELECTPROJECT_H
