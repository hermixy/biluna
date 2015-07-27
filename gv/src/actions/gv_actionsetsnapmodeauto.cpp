/*****************************************************************
 * $Id: gv_actionsetsnapmodeauto.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 23, 2008 2:16:40 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmodeauto.cpp 7179
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnapmodeauto.h"


RB_GuiAction* GV_ActionSetSnapModeAuto::action = NULL;


/**
 * Constructor.
 */
GV_ActionSetSnapModeAuto::GV_ActionSetSnapModeAuto(
		GV_Drawing& dwg, GV_GraphicsView& gv)
    : GV_ActionSetSnapMode(dwg, gv, GV2::ActionSnapAuto) {

}



RB_GuiAction* GV_ActionSetSnapModeAuto::createGuiAction() {
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionSetSnapModeAuto::getName(),
                "snapauto", tr("&Auto"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setStatusTip(tr("Auto snap"));
        action->setCommand("autosnap,aut,sa");
        action->setKeycode("sa");
        action->setFactory(GV_ActionSetSnapModeAuto::factory);

        actionGroup->addAction(action);
    }

    return action;
}



RB_Action* GV_ActionSetSnapModeAuto::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
        
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionSetSnapModeAuto(
        		*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        if (action!=NULL) {
            action->setChecked(true);
        }
        return a;
    }

    return NULL;
}
