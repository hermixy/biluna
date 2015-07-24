/*****************************************************************
 * $Id: srm_actionactivityreport.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONACTIVITYREPORT_H
#define SRM_ACTIONACTIVITYREPORT_H

#include "rb_action.h"



/**
 * View contact by customer report
 */
class SRM_ActionActivityReport : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionActivityReport();
    virtual ~SRM_ActionActivityReport() { }

    static RB_String getName() {
        return "View activity report";
    }

    virtual RB_String name() {
        return SRM_ActionActivityReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONACTIVITYREPORT_H
