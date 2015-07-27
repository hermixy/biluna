/*****************************************************************
 * $Id: gv_actionzoomout.cpp 1263 2010-07-30 14:07:45Z rutger $
 * Created: Oct 5, 2008 12:56:25 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomout.cpp 7089
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomout.h"


GV_ActionZoomOut::GV_ActionZoomOut(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionZoomIn(dwg, gv /*, RS2::Out */) {
}

RB_GuiAction* GV_ActionZoomOut::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomIn::getName(),
                    QIcon(":/images/icons/zoom_out.png"),
                    tr("Zoom &Out"));
        action->setStatusTip(tr("Zooms out"));
        action->setCommand("zoomout,zo");
        action->setKeycode("-,zo");
        action->setFactory(GV_ActionZoomOut::factory);
    }

    return action;
}


void GV_ActionZoomOut::init(int status) {
    GV_ActionBase::init(status);
    trigger(); // executed immediately
}

RB_Action* GV_ActionZoomOut::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionZoomOut(*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void GV_ActionZoomOut::trigger() {
    getGraphicsView()->zoomOut();
    finish();
}
