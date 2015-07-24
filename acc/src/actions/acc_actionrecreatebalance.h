/*****************************************************************
 * $Id: acc_actionrecreatebalance.h 1455 2011-09-19 16:19:22Z rutger $
 * Created: Sept 19, 2011 11:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONRECREATEBALANCE_H
#define ACC_ACTIONRECREATEBALANCE_H

#include "rb_action.h"


/**
 * Maintenance action, recreates the trial balance.
 * The budget values will be lost
 */
class ACC_ActionRecreateBalance : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionRecreateBalance();
    virtual ~ACC_ActionRecreateBalance() {}

    static RB_String getName() {
        return "Recreate balance";
    }

    virtual RB_String name() {
        return ACC_ActionRecreateBalance::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONRECREATEBALANCE_H
