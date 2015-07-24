/*****************************************************************
 * $Id: srm_actionsupplierreport.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Jun 1, 2012 21:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONSUPPLIERREPORT_H
#define SRM_ACTIONSUPPLIERREPORT_H

#include "rb_action.h"



/**
 * View customer report
 */
class SRM_ActionSupplierReport : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionSupplierReport();
    virtual ~SRM_ActionSupplierReport() { }

    static RB_String getName() {
        return "View supplier report";
    }

    virtual RB_String name() {
        return SRM_ActionSupplierReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONSUPPLIERREPORT_H
