/*****************************************************************
 * $Id: gv_actionzoomauto.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 12:39:59 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionautozoom.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomauto.h"
//Added by qt3to4:
// #include <QPixmap>


/**
 * Constructor.
 *
 * @param keepAspectRatio true: keep same zoom value for x/y.
 *                        false: adjust both x and y individually
 */
GV_ActionZoomAuto::GV_ActionZoomAuto(GV_Drawing& dwg, GV_GraphicsView& gv,
               bool keepAspectRatio) : GV_ActionBase(dwg, gv) {

    this->keepAspectRatio = keepAspectRatio;
}


RB_GuiAction* GV_ActionZoomAuto::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomAuto::getName(),
        			"zoomauto",
                    tr("&Auto Zoom"));
        action->setStatusTip(tr("Zooms automatic"));
        action->setCommand("zoomauto,za");
        action->setKeycode("za");
        action->setFactory(GV_ActionZoomAuto::factory);
    }
    return action;
}

/*
void GV_ActionZoomAuto::init(int status) {
    GV_ActionBase::init(status);
    trigger();
}
*/


RB_Action* GV_ActionZoomAuto::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* a = new RS_ActionZoomAuto(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }

    return NULL;
}


/*
void RS_ActionZoomAuto::trigger() {
    graphicView->zoomAuto(false, keepAspectRatio);
    finish();
}
*/
