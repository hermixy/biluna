/*****************************************************************
 * $Id: acc_actionsalesorder.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Jan 23, 2010 1:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESORDER_H
#define ACC_ACTIONSALESORDER_H

#include "rb_action.h"


/**
 * Action opens sales orders edit window
 */
class ACC_ActionSalesOrder : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesOrder();
    virtual ~ACC_ActionSalesOrder() {}

    static RB_String getName() {
        return "Sales Orders";
    }

    virtual RB_String name() {
        return ACC_ActionSalesOrder::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESORDER_H
