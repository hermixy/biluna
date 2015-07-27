/*****************************************************************
 * $Id: gv_actionsetsnaprestrictionnothing.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Nov 7, 2008 4:24:19 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsnaprestrictionnothing.cpp 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnaprestrictionnothing.h"



RB_GuiAction* GV_ActionSetSnapRestrictionNothing::action = NULL;



GV_ActionSetSnapRestrictionNothing::GV_ActionSetSnapRestrictionNothing(
		GV_Drawing& dwg, GV_GraphicsView& gv)
    : GV_ActionSetSnapRestriction(dwg, gv, GV2::ActionRestrictNothing) {
}



RB_GuiAction* GV_ActionSetSnapRestrictionNothing::createGuiAction() {
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionSetSnapRestrictionNothing::getName(),
                "restrictnothing", tr("Restrict &Nothing"));
        action->setCheckable(true);
        action->setChecked(true);
        action->setStatusTip(tr("No snap restriction"));
        action->setCommand("en");
        action->setKeycode("en");
        action->setFactory(GV_ActionSetSnapRestrictionNothing::factory);

        actionGroup->addAction(action);
    }

    return action;
}



RB_Action* GV_ActionSetSnapRestrictionNothing::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
        
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionSetSnapRestrictionNothing(
        		*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        if (action!=NULL) {
            action->setChecked(true);
        }
        return a;
    }

    return NULL;
}

