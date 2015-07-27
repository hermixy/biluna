/*****************************************************************
 * $Id: gv_actioneditcut.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:27:29 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actioneditcut.cpp 7804 
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioneditcut.h"
/*
#include "rs_modification.h"
#include "rs_clipboard.h"

#include <QPixmap>
*/
/**
 * Constructor.
 * @param undo true for undo and false for redo.
 */
GV_ActionEditCut::GV_ActionEditCut(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionEditCopy(dwg, gv, false, false) {
}



GV_ActionEditCut::~GV_ActionEditCut() {}



RB_GuiAction* GV_ActionEditCut::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionEditCut::getName(),
                	"editcutref",
                	tr("Cu&t with reference"));
        action->setStatusTip(tr("Cuts entities to the clipboard with a reference point"));
        action->setShortcut("Shift+Ctrl+X");
        action->setCommand("cutref,rt");
        action->setKeycode("rt");
        action->setFactory(GV_ActionEditCut::factory);
    }

    return action;
}



RB_Action* GV_ActionEditCut::factory() {
/*    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionEditCut(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }
*/
    return NULL;
}
