/*****************************************************************
 * $Id: peng_actionpedeqsummary.h 1377 2011-03-02 21:09:50Z rutger $
 * Created: Mar 2, 2011 10:57:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONPEDEQSUMMARY_H
#define PENG_ACTIONPEDEQSUMMARY_H

#include "rb_action.h"


/**
 * Action opens PED equipment summary MDI window
 */
class PENG_ActionPedEqSummary : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionPedEqSummary();
    virtual ~PENG_ActionPedEqSummary() {}

    static RB_String getName() {
        return "View PED equipment summary";
    }

    virtual RB_String name() {
        return PENG_ActionPedEqSummary::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONPEDEQSUMMARY_H
