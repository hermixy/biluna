/*****************************************************************
 * $Id: acc_actionpurchaseorderreport.h 1554 2012-02-22 21:36:54Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPURCHASEORDERREPORT_H
#define ACC_ACTIONPURCHASEORDERREPORT_H

#include "rb_action.h"


/**
 * Action opens purchase order report
 */
class ACC_ActionPurchaseOrderReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionPurchaseOrderReport();
    virtual ~ACC_ActionPurchaseOrderReport() {}

    static RB_String getName() {
        return "View purchase order report";
    }

    virtual RB_String name() {
        return ACC_ActionPurchaseOrderReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPURCHASEORDERREPORT_H
