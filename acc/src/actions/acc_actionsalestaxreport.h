/*****************************************************************
 * $Id: acc_actionsalestaxreport.h 1792 2012-10-15 19:54:04Z rutger $
 * Created: Oct 20, 2010 10:57:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESTAXREPORT_H
#define ACC_ACTIONSALESTAXREPORT_H

#include "rb_action.h"


/**
 * Action opens Netherlands sales tax report
 */
class ACC_ActionSalesTaxReport : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesTaxReport();
    virtual ~ACC_ActionSalesTaxReport() {}

    static RB_String getName() {
        return "View Netherlands sales tax report";
    }

    virtual RB_String name() {
        return ACC_ActionSalesTaxReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESTAXREPORT_H
