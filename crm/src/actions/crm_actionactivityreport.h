/*****************************************************************
 * $Id: crm_actionactivityreport.h 1664 2012-06-09 14:51:17Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONACTIVITYREPORT_H
#define CRM_ACTIONACTIVITYREPORT_H

#include "rb_action.h"



/**
 * View contact by customer report
 */
class CRM_ActionActivityReport : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionActivityReport();
    virtual ~CRM_ActionActivityReport() { }

    static RB_String getName() {
        return "View activity report";
    }

    virtual RB_String name() {
        return CRM_ActionActivityReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONACTIVITYREPORT_H
