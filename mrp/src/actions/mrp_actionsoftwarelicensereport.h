/*****************************************************************
 * $Id: sail_actiontrackdistancereport.h 1732 2012-08-11 16:46:12Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_ACTIONSOFTWARELICENSEREPORT_H
#define MRP_ACTIONSOFTWARELICENSEREPORT_H

#include "rb_action.h"


/**
 * Action opens track distance report window
 */
class MRP_ActionSoftwareLicenseReport : public RB_Action {

    Q_OBJECT

public:
    MRP_ActionSoftwareLicenseReport();
    virtual ~MRP_ActionSoftwareLicenseReport() {}

    static RB_String getName() {
        return "Software License Report";
    }

    virtual RB_String name() {
        return MRP_ActionSoftwareLicenseReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // MRP_ACTIONSOFTWARELICENSEREPORT_H
