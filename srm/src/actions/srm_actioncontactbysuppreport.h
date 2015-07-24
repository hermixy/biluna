/*****************************************************************
 * $Id: srm_actioncontactbysuppreport.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Jun 8, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONCONTACTBYSUPPREPORT_H
#define SRM_ACTIONCONTACTBYSUPPREPORT_H

#include "rb_action.h"



/**
 * View contact by supplier report
 */
class SRM_ActionContactBySuppReport : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionContactBySuppReport();
    virtual ~SRM_ActionContactBySuppReport() { }

    static RB_String getName() {
        return "View contact by supplier report";
    }

    virtual RB_String name() {
        return SRM_ActionContactBySuppReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONCONTACTBYSUPPREPORT_H
