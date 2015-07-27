/*****************************************************************
 * $Id: gv_actionsetsnaprestrictionorthogonal.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Nov 7, 2008 4:40:55 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnaprestrictionorthogonal.cpp 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnaprestrictionorthogonal.h"



RB_GuiAction* GV_ActionSetSnapRestrictionOrthogonal::action = NULL;



GV_ActionSetSnapRestrictionOrthogonal::GV_ActionSetSnapRestrictionOrthogonal(
		GV_Drawing& dwg, GV_GraphicsView& gv)
    : GV_ActionSetSnapRestriction(dwg, gv, GV2::ActionRestrictOrthogonal) {
}



RB_GuiAction* GV_ActionSetSnapRestrictionOrthogonal::createGuiAction() {
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionSetSnapRestrictionOrthogonal::getName(),
                "restrictorthogonal", tr("Restrict &Orthogonally"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setStatusTip(tr("Restrict snapping orthogonally"));
        action->setCommand("eo");
        action->setKeycode("eo");
        action->setFactory(GV_ActionSetSnapRestrictionOrthogonal::factory);

        actionGroup->addAction(action);
    }

    return action;
}



RB_Action* GV_ActionSetSnapRestrictionOrthogonal::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
        
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionSetSnapRestrictionOrthogonal(
        		*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        if (action!=NULL) {
            action->setChecked(true);
        }
        return a;
    }

    return NULL;

}

