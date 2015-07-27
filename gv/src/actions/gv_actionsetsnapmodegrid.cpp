/*****************************************************************
 * $Id: gv_actionsetsnapmodegrid.cpp 1158 2010-05-18 21:28:06Z rutger $
 * Created: Oct 23, 2008 2:24:08 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmodegrid.cpp 7179
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnapmodegrid.h"



RB_GuiAction* GV_ActionSetSnapModeGrid::action = NULL;



/**
 * Constructor.
 */
GV_ActionSetSnapModeGrid::GV_ActionSetSnapModeGrid(
		GV_Drawing& dwg, GV_GraphicsView& gv)
    	: GV_ActionSetSnapMode(dwg, gv, GV2::ActionSnapGrid) {
}



RB_GuiAction* GV_ActionSetSnapModeGrid::createGuiAction() {
    

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionSetSnapModeGrid::getName(),
            
                "snapgrid", tr("&Grid"));
        action->setCheckable(true);
        action->setChecked(true);
        action->setStatusTip(tr("Snap to grid points"));
        action->setCommand("grid,gri,sg");
        action->setKeycode("sg");
        action->setFactory(GV_ActionSetSnapModeGrid::factory);

        actionGroup->addAction(action);
    }

    return action;
}



RB_Action* GV_ActionSetSnapModeGrid::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
        
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionSetSnapModeGrid(
        		*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        if (action!=NULL) {
            action->setChecked(true);
        }
        return a;
    }

    return NULL;
}
