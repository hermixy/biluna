/*****************************************************************
 * $Id: pcalc_actionmaterial.cpp 0 2016-06-01 15:43:20Z rutger $
 * Created: June 1, 2016 15:43:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONMATERIAL_H
#define PCALC_ACTIONMATERIAL_H

#include "rb_action.h"


/**
 * Action opens gasket property window
 */
class PCALC_ActionMaterial : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionMaterial();
    virtual ~PCALC_ActionMaterial() {}

    static RB_String getName() {
        return "Material";
    }

    virtual RB_String name() {
        return PCALC_ActionMaterial::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONMATERIAL_H
