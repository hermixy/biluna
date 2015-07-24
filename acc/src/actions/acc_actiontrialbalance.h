/*****************************************************************
 * $Id: acc_actiontrialbalance.h 1692 2012-07-09 20:15:01Z rutger $
 * Created: Oct 13, 2010 10:57:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONTRIALBALANCE_H
#define ACC_ACTIONTRIALBALANCE_H

#include "rb_action.h"


/**
 * Action opens trial balance view widget
 */
class ACC_ActionTrialBalance : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionTrialBalance();
    virtual ~ACC_ActionTrialBalance() {}

    static RB_String getName() {
        return "View trial balance";
    }

    virtual RB_String name() {
        return ACC_ActionTrialBalance::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONTRIALBALANCE_H
