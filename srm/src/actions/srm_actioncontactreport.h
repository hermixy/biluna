/*****************************************************************
 * $Id: srm_actioncontactreport.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Jun 7, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONCONTACTREPORT_H
#define SRM_ACTIONCONTACTREPORT_H

#include "rb_action.h"



/**
 * View contact report
 */
class SRM_ActionContactReport : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionContactReport();
    virtual ~SRM_ActionContactReport() { }

    static RB_String getName() {
        return "View contact report";
    }

    virtual RB_String name() {
        return SRM_ActionContactReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONCUSTOMERREPORT_H
