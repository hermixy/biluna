/*****************************************************************
 * $Id: gv_actionzoomin.cpp 1263 2010-07-30 14:07:45Z rutger $
 * Created: Oct 5, 2008 12:49:17 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomin.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomin.h"
//#include <QPixmap>


/**
 * Default constructor.
 * @param direction In for zooming in, Out for zooming out.
 * @param axis Axis that are affected by the zoom (OnlyX, OnlyY or Both)
 */
GV_ActionZoomIn::GV_ActionZoomIn(GV_Drawing& dwg, GV_GraphicsView& gv /*,
                                 RS2::ZoomDirection direction,
                                 RS2::Axis axis,
                                 const GV_Vector& center */)
        :GV_ActionBase(dwg, gv) {

/*    this->direction = direction;
    this->axis = axis;
    this->center = center;
*/
}

RB_GuiAction* GV_ActionZoomIn::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomIn::getName(),
                    QIcon(":/images/icons/zoom_in.png"),
                    tr("Zoom &In"));
        action->setStatusTip(tr("Zooms in"));
        action->setCommand("zoomin,zi");
        action->setKeycode("+,zi");
        action->setFactory(GV_ActionZoomIn::factory);
    }

    return action;
}


void GV_ActionZoomIn::init(int status) {
    GV_ActionBase::init(status);
    trigger(); // executed immediately
}

RB_Action* GV_ActionZoomIn::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionZoomIn(*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void GV_ActionZoomIn::trigger() {
//    switch (axis) {
//    case RS2::OnlyX:
//        if (direction==RS2::In) {
//            graphicView->zoomInX();
//        } else {
//            graphicView->zoomOutX();
//        }
//        break;
//
//    case RS2::OnlyY:
//        if (direction==RS2::In) {
//            graphicView->zoomInY();
//        } else {
//            graphicView->zoomOutY();
//        }
//        break;
//
//    case RS2::Both:
//        if (direction==RS2::In) {
//            graphicView->zoomIn(1.25, center);
//        } else {
//            graphicView->zoomOut(1.25, center);
//        }
//        break;
//    }
    getGraphicsView()->zoomIn();
    finish();
}
