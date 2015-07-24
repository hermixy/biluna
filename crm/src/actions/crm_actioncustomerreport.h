/*****************************************************************
 * $Id: crm_actioncustomerreport.h 1658 2012-06-04 13:06:33Z rutger $
 * Created: Jun 1, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONCUSTOMERREPORT_H
#define CRM_ACTIONCUSTOMERREPORT_H

#include "rb_action.h"



/**
 * View customer report
 */
class CRM_ActionCustomerReport : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionCustomerReport();
    virtual ~CRM_ActionCustomerReport() { }

    static RB_String getName() {
        return "View customer report";
    }

    virtual RB_String name() {
        return CRM_ActionCustomerReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONCUSTOMERREPORT_H
