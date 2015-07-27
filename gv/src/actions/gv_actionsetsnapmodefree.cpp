/*****************************************************************
 * $Id: gv_actionsetsnapmodefree.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 23, 2008 2:20:56 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmodefree.cpp 7179
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnapmodefree.h"


RB_GuiAction* GV_ActionSetSnapModeFree::action = NULL;


/**
 * Constructor.
 */
GV_ActionSetSnapModeFree::GV_ActionSetSnapModeFree(
		GV_Drawing& dwg, GV_GraphicsView& gv)
    	: GV_ActionSetSnapMode(dwg, gv, GV2::ActionSnapFree) {
}



RB_GuiAction* GV_ActionSetSnapModeFree::createGuiAction() {
    

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionSetSnapModeFree::getName(),
                "snapfree", tr("&Free"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setStatusTip(tr("Free positioning"));
        action->setCommand("free,fre,sf,os");
        action->setKeycode("sf,os");
        action->setFactory(GV_ActionSetSnapModeFree::factory);
        
        actionGroup->addAction(action);
    }

    return action;
}



RB_Action* GV_ActionSetSnapModeFree::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
    
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionSetSnapModeFree(
        		*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        if (action!=NULL) {
            action->setChecked(true);
        }
        return a;
    }

    return NULL;
}
