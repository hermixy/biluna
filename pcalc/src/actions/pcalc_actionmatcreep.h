/*****************************************************************
 * $Id: pcalc_actionmatcreep.h 0 2016-06-01 10:23:20Z rutger $
 * Created: June 1, 2016 10:23:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONMATCREEP_H
#define PCALC_ACTIONMATCREEP_H

#include "rb_action.h"


/**
 * Action opens gasket property window
 */
class PCALC_ActionMatCreep : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionMatCreep();
    virtual ~PCALC_ActionMatCreep() {}

    static RB_String getName() {
        return "Material Creep";
    }

    virtual RB_String name() {
        return PCALC_ActionMatCreep::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONMATCREEP_H
