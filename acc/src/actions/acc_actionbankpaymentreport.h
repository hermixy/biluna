/*****************************************************************
 * $Id: sail_actiontrackdistancereport.h 1732 2012-08-11 16:46:12Z rutger $
 * Created: Oct 14, 2015 21:17:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONBANKPAYMENTREPORT_H
#define ACC_ACTIONBANKPAYMENTREPORT_H

#include "rb_action.h"


/**
 * Action opens track distance report window
 */
class ACC_ActionBankPaymentReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionBankPaymentReport();
    virtual ~ACC_ActionBankPaymentReport() {}

    static RB_String getName() {
        return "Bank Payment Report";
    }

    virtual RB_String name() {
        return ACC_ActionBankPaymentReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONBANKPAYMENTREPORT_H
