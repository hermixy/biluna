/*****************************************************************
 * $Id: acc_actionpaymentterm.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 17, 2010 5:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONPAYMENTTERM_H
#define ACC_ACTIONPAYMENTTERM_H

#include "rb_action.h"


/**
 * Action opens payment terms edit dialog
 */
class ACC_ActionPaymentTerm : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionPaymentTerm();
    virtual ~ACC_ActionPaymentTerm() {}

    static RB_String getName() {
        return "Payment terms";
    }

    virtual RB_String name() {
        return ACC_ActionPaymentTerm::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONPAYMENTTERM_H
