/*****************************************************************
 * $Id: gv_actioneditredo.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:49:46 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditredo.h 7167
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioneditredo.h"



/**
 * Constructor.
 * @param undo true for undo and false for redo.
 */
GV_ActionEditRedo::GV_ActionEditRedo(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionEditUndo(dwg, gv, false) {
}



GV_ActionEditRedo::~GV_ActionEditRedo() {}


RB_GuiAction* GV_ActionEditRedo::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionEditRedo::getName(),
        			"editredo", 
        			tr("&Redo"));
        action->setStatusTip(tr("Redoes last action"));
        action->setCommand("redo,uu,r");
        action->setKeycode("uu");
        action->setShortcut("Ctrl+Shift+Z,Ctrl+Y");
        action->setFactory(GV_ActionEditRedo::factory);
    }

    return action;
}


RB_Action* GV_ActionEditRedo::factory() {
/*    GV_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a =
            new RS_ActionEditRedo(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }
*/
    return NULL;
}

