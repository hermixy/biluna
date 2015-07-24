/*****************************************************************
 * $Id: acc_actionallocnreport.h 2141 2014-04-07 17:53:21Z rutger $
 * Created: Jun 30, 2011 7:57:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONALLOCNREPORT_H
#define ACC_ACTIONALLOCNREPORT_H

#include "rb_action.h"


/**
 * Action opens allocation report MDI window
 */
class ACC_ActionAllocnReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionAllocnReport();
    virtual ~ACC_ActionAllocnReport() {}

    static RB_String getName() {
        return "View allocation/reconciliation report";
    }

    virtual RB_String name() {
        return ACC_ActionAllocnReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONALLOCNREPORT_H
