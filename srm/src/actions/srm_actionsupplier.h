/*****************************************************************
 * $Id: srm_actionsupplier.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 18, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONSUPPLIER_H
#define SRM_ACTIONSUPPLIER_H

#include "rb_action.h"



/**
 * Edit/view customer
 */
class SRM_ActionSupplier : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionSupplier();
    virtual ~SRM_ActionSupplier() { }

    static RB_String getName() {
        return "Edit/view supplier";
    }

    virtual RB_String name() {
        return SRM_ActionSupplier::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONSUPPLIER_H
