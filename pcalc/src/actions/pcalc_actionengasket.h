/*****************************************************************
 * $Id: pcalc_actionen1591flange.h 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONENGASKET_H
#define PCALC_ACTIONENGASKET_H

#include "rb_action.h"


/**
 * Action opens gasket property window
 */
class PCALC_ActionENGasket : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionENGasket();
    virtual ~PCALC_ActionENGasket() {}

    static RB_String getName() {
        return "EN gasket properties";
    }

    virtual RB_String name() {
        return PCALC_ActionENGasket::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONENGASKET_H
