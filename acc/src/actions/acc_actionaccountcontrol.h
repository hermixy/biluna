/*****************************************************************
 * $Id: acc_actionaccountcontrol.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Nov 11, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONACCOUNTCONTROL_H
#define ACC_ACTIONACCOUNTCONTROL_H

#include "rb_action.h"


/**
 * Action opens account control edit dialog
 */
class ACC_ActionAccountControl : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionAccountControl();
    virtual ~ACC_ActionAccountControl() {}

    static RB_String getName() {
        return "Account Control";
    }

    virtual RB_String name() {
        return ACC_ActionAccountControl::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONACCOUNTGROUP_H
