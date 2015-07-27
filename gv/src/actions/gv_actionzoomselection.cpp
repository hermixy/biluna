/*****************************************************************
 * $Id: gv_actionzoomselection.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:15:01 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomselection.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomselection.h"
//Added by qt3to4:
// #include <QPixmap>


/**
 * Constructor.
 */
GV_ActionZoomSelection::GV_ActionZoomSelection(GV_Drawing& dwg, 
					GV_GraphicsView& gv)
                    : GV_ActionBase(dwg, gv) {
}


RB_GuiAction* GV_ActionZoomSelection::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomSelection::getName(),
        			"zoomselection",
        			tr("Zoom to &Selection"));
        action->setStatusTip(tr("Zooms to current selection"));
        action->setCommand("zoomselection,zs");
        action->setKeycode("zs");
        action->setFactory(GV_ActionZoomSelection::factory);
    }
    return action;
}

/*
void GV_ActionZoomSelection::init(int status) {
    GV_ActionBase::init(status);
    trigger();
}
*/


RB_Action* GV_ActionZoomSelection::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* a = new RS_ActionZoomSelection(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }

    return NULL;
}


/*
void RS_ActionZoomSelection::trigger() {
    graphicView->zoomSelection();
    finish();
}
*/
