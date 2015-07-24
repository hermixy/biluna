/*****************************************************************
 * $Id: peng_actioninsulation.h 2165 2014-09-24 19:22:36Z rutger $
 * Created: Sep 15, 2014 4:14:42 PM - rutger
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONINSULATION_H
#define PENG_ACTIONINSULATION_H

#include "rb_action.h"


/**
 * Action opens insulation table edit/selection dialog
 */
class PENG_ActionInsulation : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionInsulation();
    virtual ~PENG_ActionInsulation() {}

    static RB_String getName() {
        return "Insulation Table";
    }

    virtual RB_String name() {
        return PENG_ActionInsulation::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONINSULATION_H
