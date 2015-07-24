/*****************************************************************
 * $Id: peng_actionclose.h 2185 2014-10-09 15:09:35Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONCLOSE_H
#define PCALC_ACTIONCLOSE_H

#include "rb_action.h"


/**
 * Action closes the perspective
 */
class PCALC_ActionClose : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionClose();
    virtual ~PCALC_ActionClose() {}

    static RB_String getName() {
        return "Close perspective";
    }

    virtual RB_String name() {
        return PCALC_ActionClose::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONCLOSE_H
