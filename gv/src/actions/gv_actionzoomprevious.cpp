/*****************************************************************
 * $Id: gv_actionzoomprevious.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:04:34 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomprevious.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomprevious.h"
//Added by qt3to4:
//#include <QPixmap>


/**
 * Constructor.
 */
GV_ActionZoomPrevious::GV_ActionZoomPrevious(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {
}


RB_GuiAction* GV_ActionZoomPrevious::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomPrevious::getName(),
            "zoomprevious",
                             tr("&Previous View"));
        action->setStatusTip(tr("Shows previous view"));
        action->setCommand("zoomprevious,zoomprev,zv");
        action->setKeycode("zv");
        action->setFactory(GV_ActionZoomPrevious::factory);
    }
    return action;
}

/*
void GV_ActionZoomPrevious::init(int status) {
    GV_ActionBase::init(status);
    trigger();
}
*/


RB_Action* GV_ActionZoomPrevious::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* a = new RS_ActionZoomPrevious(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }

    return NULL;
}


/*
void RS_ActionZoomPrevious::trigger() {
    graphicView->zoomPrevious();
    finish();
}
*/
// EOF
