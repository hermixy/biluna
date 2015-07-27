/*****************************************************************
 * $Id: gv_actioneditcopyautoref.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:16:11 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcopyautoref.h 7804
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioneditcopyautoref.h"



GV_ActionEditCopyAutoRef::GV_ActionEditCopyAutoRef(GV_Drawing& dwg, 
		GV_GraphicsView& gv) : GV_ActionEditCopy(dwg, gv, true, true) {
}



GV_ActionEditCopyAutoRef::~GV_ActionEditCopyAutoRef() {}



RB_GuiAction* GV_ActionEditCopyAutoRef::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionEditCopyAutoRef::getName(),
                	"editcopy",
                	tr("&Copy"));
        action->setStatusTip(tr("Copies entities to the clipboard"));
        action->setShortcut("Ctrl+C");
        action->setCommand("copy,cp");
        action->setKeycode("cp");
        action->setFactory(GV_ActionEditCopyAutoRef::factory);
    }

    return action;
}



RB_Action* GV_ActionEditCopyAutoRef::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* action = new RS_ActionEditCopyAutoRef(*container, *graphicView);
//        graphicView->setCurrentAction(action);
//        return action;
//    }

    return NULL;
}
