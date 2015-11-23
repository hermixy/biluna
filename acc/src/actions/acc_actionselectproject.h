/*****************************************************************
 * $Id: acc_actionselectproject.h 1336 2010-12-22 21:35:49Z rutger $
 * Created: Dec 21, 2009 21:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSELECTPROJECT_H
#define ACC_ACTIONSELECTPROJECT_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Select project from database
 */
class ACC_ActionSelectProject : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSelectProject();
    virtual ~ACC_ActionSelectProject();

    static RB_String getName() {
        return "Select project";
    }

    virtual RB_String name() {
        return ACC_ActionSelectProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSELECTPROJECT_H
