/*****************************************************************
 * $Id: acc_actionpurchaseorder.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: May 26, 2010 1:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPURCHASEORDER_H
#define ACC_ACTIONPURCHASEORDER_H

#include "rb_action.h"


/**
 * Action opens purchase orders edit window
 */
class ACC_ActionPurchaseOrder : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionPurchaseOrder();
    virtual ~ACC_ActionPurchaseOrder() {}

    static RB_String getName() {
        return "Sales Orders";
    }

    virtual RB_String name() {
        return ACC_ActionPurchaseOrder::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPURCHASEORDER_H
