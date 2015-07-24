/*****************************************************************
 * $Id: acc_actioncostcenterbudget.h 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 21, 2012 14:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCOSTCENTERBUDGET_H
#define ACC_ACTIONCOSTCENTERBUDGET_H

#include "rb_action.h"


/**
 * Action opens cost center and budget edit dialog
 */
class ACC_ActionCostCenterBudget : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCostCenterBudget();
    virtual ~ACC_ActionCostCenterBudget() {}

    static RB_String getName() {
        return "Cost Center and Budget";
    }

    virtual RB_String name() {
        return ACC_ActionCostCenterBudget::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCOSTCENTERBUDGET_H
