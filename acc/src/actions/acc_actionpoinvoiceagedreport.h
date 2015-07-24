/*****************************************************************
 * $Id: acc_actionpoinvoiceagedreport.h 1329 2010-12-11 18:27:47Z rutger $
 * Created: Dec 8, 2010 12:29:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPOINVOICEAGEDREPORT_H
#define ACC_ACTIONPOINVOICEAGEDREPORT_H

#include "rb_action.h"


/**
 * Action opens purchase invoice aged report MDI window
 */
class ACC_ActionPoInvoiceAgedReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionPoInvoiceAgedReport();
    virtual ~ACC_ActionPoInvoiceAgedReport() {}

    static RB_String getName() {
        return "View purchase invoice aged report";
    }

    virtual RB_String name() {
        return ACC_ActionPoInvoiceAgedReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPOINVOICEAGEDREPORT_H
