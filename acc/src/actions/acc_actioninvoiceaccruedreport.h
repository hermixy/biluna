/*****************************************************************
 * $Id: acc_actioninvoiceaccruedreport.h 2202 2015-01-11 19:39:36Z rutger $
 * Created: Jan 11, 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONINVOICEACCRUEDREPORT_H
#define ACC_ACTIONINVOICEACCRUEDREPORT_H

#include "rb_action.h"


/**
 * Action opens invoice (sales/purchases) accrued report
 */
class ACC_ActionInvoiceAccruedReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionInvoiceAccruedReport();
    virtual ~ACC_ActionInvoiceAccruedReport() {}

    static RB_String getName() {
        return "Invoice Accrued Report";
    }

    virtual RB_String name() {
        return ACC_ActionInvoiceAccruedReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONINVOICEACCRUEDREPORT_H
