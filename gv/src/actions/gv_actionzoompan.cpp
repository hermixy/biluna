/*****************************************************************
 * $Id: gv_actionzoompan.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:00:44 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoompan.cpp 10508
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoompan.h"

//#include <QPixmap>
/*
#include "rs_snapper.h"
#include "rs_point.h"
*/

GV_ActionZoomPan::GV_ActionZoomPan(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {}


RB_GuiAction* GV_ActionZoomPan::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(
            GV_ActionZoomPan::getName(), 
            "zoompan",
            tr("&Pan Zoom"));
        action->setStatusTip(tr("Realtime panning"));
        action->setCommand("zoompan,zp");
        action->setKeycode("zp");
        action->setFactory(GV_ActionZoomPan::factory);
    }                                  
    return action;
}

/*
void GV_ActionZoomPan::init(int status) {
    GV_ActionBase::init(status);
    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
    x1 = y1 = x2 = y2 = -1;
    //graphicView->saveView();
}
*/


RB_Action* GV_ActionZoomPan::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* a = new RS_ActionZoomPan(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }

    return NULL;
}


/*
void RS_ActionZoomPan::trigger() {
    if (x1>=0) {
        graphicView->zoomPan(x2-x1, y2-y1);
        x1 = x2;
        y1 = y2;
    }
}



void RS_ActionZoomPan::mouseMoveEvent(RS_MouseEvent* e) {
    x2 = e->x();
    y2 = e->y();
    if (getStatus()==1 && (abs(x2-x1)>7 || abs(y2-y1)>7)) {
        trigger();
    }
}



void RS_ActionZoomPan::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::MidButton ||
            RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        x1 = e->x();
        y1 = e->y();
        setStatus(1);
    }
}



void RS_ActionZoomPan::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::MidButton) {
        init(-1);
    } else {
        setStatus(0);
    }
}

void RS_ActionZoomPan::updateMouseButtonHints() {
    switch (getStatus()) {
    case 0:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Drag and drop to pan around"), tr("Cancel"));
        break;
    case 1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Drop to stop panning"), tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionZoomPan::updateMouseCursor() {
#ifndef __APPLE__
    graphicView->setMouseCursor(RS2::SizeAllCursor);
#endif
}
*/

