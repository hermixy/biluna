/*****************************************************************
 * $Id: acc_actioncostcenterbalance.h 1786 2012-10-01 20:24:24Z rutger $
 * Created: Sept 29, 2012 10:18:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCOSTCENTERBALANCE_H
#define ACC_ACTIONCOSTCENTERBALANCE_H

#include "rb_action.h"


/**
 * Action opens cost center balance per customer and supplier report window
 */
class ACC_ActionCostCenterBalance : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionCostCenterBalance();
    virtual ~ACC_ActionCostCenterBalance() {}

    static RB_String getName() {
        return "Cost Center Balance";
    }

    virtual RB_String name() {
        return ACC_ActionCostCenterBalance::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCOSTCENTERBALANCE_H
