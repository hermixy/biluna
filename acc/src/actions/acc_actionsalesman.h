/*****************************************************************
 * $Id: acc_actionsalesman.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 20, 2010 12:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESMAN_H
#define ACC_ACTIONSALESMAN_H

#include "rb_action.h"


/**
 * Action opens salesman edit dialog
 */
class ACC_ActionSalesMan : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesMan();
    virtual ~ACC_ActionSalesMan() {}

    static RB_String getName() {
        return "Currency";
    }

    virtual RB_String name() {
        return ACC_ActionSalesMan::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESMAN_H
