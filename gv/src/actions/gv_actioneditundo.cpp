/*****************************************************************
 * $Id: gv_actioneditundo.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:55:22 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditundo.h 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioneditundo.h"
//#include "rs_snapper.h"
////Added by qt3to4:
//#include <QPixmap>

/**
 * Constructor.
 * @param undo true for undo and false for redo.
 */
GV_ActionEditUndo::GV_ActionEditUndo(GV_Drawing& dwg, GV_GraphicsView& gv,
                                     bool undo) : GV_ActionBase(dwg, gv) {
    this->undo = undo;
}



GV_ActionEditUndo::~GV_ActionEditUndo() {}


RB_GuiAction* GV_ActionEditUndo::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionEditUndo::getName(),
        			"editundo", 
        			tr("&Undo"));
        action->setStatusTip(tr("Undoes last action"));
        action->setCommand("undo,oops,oo,u");
        action->setKeycode("oo");
        action->setShortcut("Ctrl+Z");
        action->setFactory(GV_ActionEditUndo::factory);
    }

    return action;
}


RB_Action* GV_ActionEditUndo::factory() {
/*    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a =
            new RS_ActionEditUndo(*container, *graphicView, true);
        graphicView->setCurrentAction(a);
        return a;
    }
*/
    return NULL;
}
/*
void RS_ActionEditUndo::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



void RS_ActionEditUndo::trigger() {
    if (undo) {
        document->undo();
    } else {
        document->redo();
    }

    document->updateInserts();

    graphicView->redraw();

    finish();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}
*/
