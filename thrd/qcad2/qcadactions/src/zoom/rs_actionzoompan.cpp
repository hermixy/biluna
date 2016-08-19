/****************************************************************************
** $Id: rs_actionzoompan.cpp 10508 2008-04-17 18:05:05Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "rs_actionzoompan.h"

#include <QPixmap>

#include "rs_snapper.h"
#include "rs_point.h"


RS_ActionZoomPan::RS_ActionZoomPan(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_ActionInterface( container, graphicView) {}


RS_GuiAction* RS_ActionZoomPan::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(
            RS_ActionZoomPan::getName(), 
            "zoompan",
            tr("&Pan Zoom"));
        action->setStatusTip(tr("Realtime panning"));
        action->setCommand("zoompan,zp");
        action->setKeycode("zp");
        action->setFactory(RS_ActionZoomPan::factory);
    }                                  
    return action;
}


void RS_ActionZoomPan::init(int status) {
    RS_ActionInterface::init(status);
    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
    x1 = y1 = x2 = y2 = -1;
    //graphicView->saveView();
}



RS_ActionInterface* RS_ActionZoomPan::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionZoomPan(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



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

