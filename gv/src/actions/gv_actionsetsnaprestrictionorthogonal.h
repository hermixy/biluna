/*****************************************************************
 * $Id: gv_actionsetsnaprestrictionorthogonal.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Nov 7, 2008 4:44:42 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnaprestriction.h 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSETSNAPRESTRICTIONORTHOGONAL_H
#define GV_ACTIONSETSNAPRESTRICTIONORTHOGONAL_H

#include "gv_actionsetsnaprestriction.h"


/**
 * This action disables the current snap restriction.
 */
class GV_ActionSetSnapRestrictionOrthogonal : public GV_ActionSetSnapRestriction {
	
    Q_OBJECT
    
public:
    GV_ActionSetSnapRestrictionOrthogonal(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionSetSnapRestrictionOrthogonal() {}

    static RB_String getName() {
        return "Orthogonal Snap Restriction";
    }

    virtual RB_String name() {
        return GV_ActionSetSnapRestrictionOrthogonal::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

protected:
    static RB_GuiAction* action;
};

#endif
