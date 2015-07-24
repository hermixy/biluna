/*****************************************************************
 * $Id: peng_actionfluidstability.h 2154 2014-07-30 19:44:11Z rutger $
 * Created: July 29, 2014 20:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONFLUIDSTABILITY_H
#define PENG_ACTIONFLUIDSTABILITY_H

#include "rb_action.h"


/**
 * Action opens fluid stability state edit dialog
 */
class PENG_ActionFluidStability : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionFluidStability();
    virtual ~PENG_ActionFluidStability() {}

    static RB_String getName() {
        return "Fluid Stability";
    }

    virtual RB_String name() {
        return PENG_ActionFluidStability::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONFLUIDSTABILITY_H
