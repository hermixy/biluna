/*****************************************************************
 * $Id: acc_actionsoinvoiceagedreport.h 1329 2010-12-11 18:27:47Z rutger $
 * Created: Dec 8, 2010 12:40:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSOINVOICEAGEDREPORT_H
#define ACC_ACTIONSOINVOICEAGEDREPORT_H

#include "rb_action.h"


/**
 * Action opens sales invoice aged report MDI window
 */
class ACC_ActionSoInvoiceAgedReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSoInvoiceAgedReport();
    virtual ~ACC_ActionSoInvoiceAgedReport() {}

    static RB_String getName() {
        return "View sales invoice aged report";
    }

    virtual RB_String name() {
        return ACC_ActionSoInvoiceAgedReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSOINVOICEAGEDREPORT_H
