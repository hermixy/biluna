/*****************************************************************
 * $Id: acc_actionunitofmeasure.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONUNITOFMEASURE_H
#define ACC_ACTIONUNITOFMEASURE_H

#include "rb_action.h"


/**
 * Action opens unit of measurement edit dialog
 */
class ACC_ActionUnitOfMeasure : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionUnitOfMeasure();
    virtual ~ACC_ActionUnitOfMeasure() {}

    static RB_String getName() {
        return "Unit of Measurement";
    }

    virtual RB_String name() {
        return ACC_ActionUnitOfMeasure::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONUNITOFMEASURE_H
