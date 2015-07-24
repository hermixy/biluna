/*****************************************************************
 * $Id: acc_actionaccountgroup.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 22, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONACCOUNTGROUP_H
#define ACC_ACTIONACCOUNTGROUP_H

#include "rb_action.h"


/**
 * Action opens account groups edit dialog
 */
class ACC_ActionAccountGroup : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionAccountGroup();
    virtual ~ACC_ActionAccountGroup() {}

    static RB_String getName() {
        return "Account Groups";
    }

    virtual RB_String name() {
        return ACC_ActionAccountGroup::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONACCOUNTGROUP_H
