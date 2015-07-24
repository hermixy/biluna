/*****************************************************************
 * $Id: acc_actionsalesorderreport.h 1554 2012-02-22 21:36:54Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESORDERREPORT_H
#define ACC_ACTIONSALESORDERREPORT_H

#include "rb_action.h"


/**
 * Action opens sales order report
 */
class ACC_ActionSalesOrderReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesOrderReport();
    virtual ~ACC_ActionSalesOrderReport() {}

    static RB_String getName() {
        return "View sales order report";
    }

    virtual RB_String name() {
        return ACC_ActionSalesOrderReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESORDERREPORT_H
