/*****************************************************************
 * $Id: crm_actioncontactbycustreport.h 1664 2012-06-09 14:51:17Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONCONTACTBYCUSTREPORT_H
#define CRM_ACTIONCONTACTBYCUSTREPORT_H

#include "rb_action.h"



/**
 * View contact by customer report
 */
class CRM_ActionContactByCustReport : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionContactByCustReport();
    virtual ~CRM_ActionContactByCustReport() { }

    static RB_String getName() {
        return "View contact by customer report";
    }

    virtual RB_String name() {
        return CRM_ActionContactByCustReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONCUSTOMERREPORT_H
