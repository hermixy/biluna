/*****************************************************************
 * $Id: gv_actioneditcutautoref.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:34:41 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditcutautoref.cpp 7804
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioneditcutautoref.h"



GV_ActionEditCutAutoRef::GV_ActionEditCutAutoRef(
		GV_Drawing& dwg, GV_GraphicsView& gv)
    : GV_ActionEditCopy(dwg, gv, false, true) {
}



GV_ActionEditCutAutoRef::~GV_ActionEditCutAutoRef() {}



RB_GuiAction* GV_ActionEditCutAutoRef::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionEditCutAutoRef::getName(),
            
                "editcut",
            tr("Cu&t"));
        action->setStatusTip(tr("Cuts entities to the clipboard"));
        action->setShortcut("Ctrl+X");
        action->setCommand("cut,ct");
        action->setKeycode("ct");
        action->setFactory(GV_ActionEditCutAutoRef::factory);
    }

    return action;
}



RB_Action* GV_ActionEditCutAutoRef::factory() {
/*    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionEditCutAutoRef(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }
*/
    return NULL;
}
