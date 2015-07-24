/*****************************************************************
 * $Id: pcalc_actionen1591flange.h 2234 2015-04-30 19:23:20Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONEN1591FLANGE_H
#define PCALC_ACTIONEN1591FLANGE_H

#include "rb_action.h"


/**
 * Action opens cone strikeout design window
 */
class PCALC_ActionEN1591Flange : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionEN1591Flange();
    virtual ~PCALC_ActionEN1591Flange() {}

    static RB_String getName() {
        return "EN1591 Flange Calculation";
    }

    virtual RB_String name() {
        return PCALC_ActionEN1591Flange::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONEN1591FLANGE_H
