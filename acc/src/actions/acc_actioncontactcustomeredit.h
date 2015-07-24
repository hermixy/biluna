/*****************************************************************
 * $Id: acc_actioncontactcustomeredit.h 2153 2014-07-25 16:38:28Z rutger $
 * Created: Jan 06, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONCONTACTCUSTOMEREDIT_H
#define ACC_ACTIONCONTACTCUSTOMEREDIT_H

#include "rb_action.h"


/**
 * Action opens Contact edit customer contact MDI window
 */
class ACC_ActionContactCustomerEdit: public RB_Action {

    Q_OBJECT

public:
    ACC_ActionContactCustomerEdit();
    virtual ~ACC_ActionContactCustomerEdit() {}

    static RB_String getName() {
        return "Customer Contact";
    }

    virtual RB_String name() {
        return ACC_ActionContactCustomerEdit::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONCONTACTCUSTOMEREDIT_H
