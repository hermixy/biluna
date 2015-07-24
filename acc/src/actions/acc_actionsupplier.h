/*****************************************************************
 * $Id: acc_actionsupplier.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Feb 10, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSUPPLIER_H
#define ACC_ACTIONSUPPLIER_H

#include "rb_action.h"


/**
 * Action opens supplier detail window
 */
class ACC_ActionSupplier : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSupplier();
    virtual ~ACC_ActionSupplier() {}

    static RB_String getName() {
        return "Supplier";
    }

    virtual RB_String name() {
        return ACC_ActionSupplier::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSUPPLIER_H
