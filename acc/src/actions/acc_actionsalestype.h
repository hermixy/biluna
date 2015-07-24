/*****************************************************************
 * $Id: acc_actionsalestype.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONSALESTYPE_H
#define ACC_ACTIONSALESTYPE_H

#include "rb_action.h"


/**
 * Action opens sales types or price list edit dialog
 */
class ACC_ActionSalesType : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionSalesType();
    virtual ~ACC_ActionSalesType() {}

    static RB_String getName() {
        return "Units of Measurement";
    }

    virtual RB_String name() {
        return ACC_ActionSalesType::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONSALESTYPE_H
