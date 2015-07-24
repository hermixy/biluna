/*****************************************************************
 * $Id: crm_actioncustomer.h 1550 2012-02-18 17:36:06Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONCUSTOMER_H
#define CRM_ACTIONCUSTOMER_H

#include "rb_action.h"

class RB_MmSource;


/**
 * Edit/view customer
 */
class CRM_ActionCustomer : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionCustomer();
    virtual ~CRM_ActionCustomer() { }

    static RB_String getName() {
        return "Edit/view customer";
    }

    virtual RB_String name() {
        return CRM_ActionCustomer::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONSELECTPROJECT_H
