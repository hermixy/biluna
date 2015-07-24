/*****************************************************************
 * $Id: peng_actionfluid.h 2149 2014-06-16 20:04:23Z rutger $
 * Created: May 23, 2014 4:14:42 PM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONFLUID_H
#define PENG_ACTIONFLUID_H

#include "rb_action.h"


/**
 * Action opens fluid edit/selection dialog
 */
class PENG_ActionFluid : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionFluid();
    virtual ~PENG_ActionFluid() {}

    static RB_String getName() {
        return "Fluid Type";
    }

    virtual RB_String name() {
        return PENG_ActionFluid::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONFLUID_H
