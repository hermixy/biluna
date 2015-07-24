/*****************************************************************
 * $Id: peng_actionweldvolcalc.h 1436 2011-07-16 16:49:02Z rutger $
 * Created: Apr 15, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONWELDVOLCALC_H
#define PENG_ACTIONWELDVOLCALC_H

#include "rb_action.h"


/**
 * Action opens weldolet weld volume calculation dialog
 */
class PENG_ActionWeldVolCalc : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionWeldVolCalc();
    virtual ~PENG_ActionWeldVolCalc() {}

    static RB_String getName() {
        return "Weldolet Weld Volume";
    }

    virtual RB_String name() {
        return PENG_ActionWeldVolCalc::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONWELDVOLCALC_H
