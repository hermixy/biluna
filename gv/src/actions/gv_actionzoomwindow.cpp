/*****************************************************************
 * $Id: gv_actionzoomwindow.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:17:59 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomwindow.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomwindow.h"

//#include "rs.h"
//#include "rs_snapper.h"
//#include "rs_point.h"
//Added by qt3to4:
//#include <QPixmap>


/**
 * Default constructor.
 *
 * @param keepAspectRatio Keep the aspect ratio. true: the factors 
 *          in x and y will stay the same. false Exactly the chosen 
 *          area will be fit to the viewport.
 */
GV_ActionZoomWindow::GV_ActionZoomWindow(GV_Drawing& dwg, GV_GraphicsView& gv, 
		bool keepAspectRatio)
        : GV_ActionBase(dwg, gv) {

    this->keepAspectRatio = keepAspectRatio;
}


RB_GuiAction* GV_ActionZoomWindow::createGuiAction() {
    
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomWindow::getName(),
            "zoomwindow",
                                  tr("&Window Zoom"));
        action->setStatusTip(tr("Zooms in a window"));
        action->setCommand("zoomwindow,zw");
        action->setKeycode("zw");
        action->setFactory(GV_ActionZoomWindow::factory);
    }

    return action;
}

/*
void GV_ActionZoomWindow::init(int status) {
    RB_DEBUG->print("GV_ActionZoomWindow::init()");

    GV_ActionBase::init(status);
    v1 = v2 = RS_Vector(false);
    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
}
*/


RB_Action* GV_ActionZoomWindow::factory() {
//    RS_EntityContainer* container = GV_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = GV_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        GV_ActionBase* a = new GV_ActionZoomWindow(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }

    return NULL;
}


/*
void GV_ActionZoomWindow::trigger() {
    RS_DEBUG->print("GV_ActionZoomWindow::trigger()");

    GV_ActionBase::trigger();

    if (v1.valid && v2.valid) {
        if (graphicView->toGuiDX(v1.distanceTo(v2))>5) {
            graphicView->zoomWindow(v1, v2, keepAspectRatio);
            init();
        }
    }
}



void GV_ActionZoomWindow::mouseMoveEvent(RS_MouseEvent* e) {
    if (getStatus()==1 && v1.valid) {
        v2 = snapPoint(e);
        clearPreview();

        if (v2.valid) {
            preview->addRectangle(v1, v2);
        }
        
        drawPreview();
    }
}



void GV_ActionZoomWindow::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case 0:
            v1 = snapPoint(e);
            if (v1.valid) {
                setStatus(1);
            }
            break;

        default:
            break;
        }
    }

    RS_DEBUG->print("GV_ActionZoomWindow::mousePressEvent(): %f %f",
                    v1.x, v1.y);
}



void GV_ActionZoomWindow::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("GV_ActionZoomWindow::mouseReleaseEvent()");

    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        if (getStatus()==1) {
            clearPreview();
            graphicView->restore();
        }
        init(getStatus()-1);
    } else if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        if (getStatus()==1) {
            v2 = snapPoint(e);
            if (v2.valid) {
                trigger();
            }
        }
    }
}



void GV_ActionZoomWindow::updateMouseButtonHints() {
    RS_DEBUG->print("GV_ActionZoomWindow::updateMouseButtonHints()");

    switch (getStatus()) {
    case 0:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Specify first edge"), tr("Cancel"));
        break;
    case 1:
        GV_DIALOGFACTORY->updateMouseWidget(tr("Specify second edge"), tr("Back"));
        break;
    default:
        GV_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void GV_ActionZoomWindow::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::MagnifierCursor);
}
*/

