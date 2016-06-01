/*****************************************************************
 * $Id: pcalc_actionenthermexp.h 0 2016-06-01 08:23:20Z rutger $
 * Created: June 1, 2016 8:23:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef PCALC_ACTIONMATTHERMEXP_H
#define PCALC_ACTIONMATTHERMEXP_H

#include "rb_action.h"


/**
 * Action opens gasket property window
 */
class PCALC_ActionMatThermExp : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionMatThermExp();
    virtual ~PCALC_ActionMatThermExp() {}

    static RB_String getName() {
        return "Material Thermal Expansion";
    }

    virtual RB_String name() {
        return PCALC_ActionMatThermExp::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONMATTHERMEXP_H
