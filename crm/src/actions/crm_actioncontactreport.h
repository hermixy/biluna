/*****************************************************************
 * $Id: crm_actioncontactreport.h 1663 2012-06-07 21:36:10Z rutger $
 * Created: Jun 7, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONCONTACTREPORT_H
#define CRM_ACTIONCONTACTREPORT_H

#include "rb_action.h"



/**
 * View contact report
 */
class CRM_ActionContactReport : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionContactReport();
    virtual ~CRM_ActionContactReport() { }

    static RB_String getName() {
        return "View contact report";
    }

    virtual RB_String name() {
        return CRM_ActionContactReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONCUSTOMERREPORT_H
