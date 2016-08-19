/****************************************************************************
** $Id: rs_actiondrawarc3p.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawarc3p.h"

#include "rs_actiondrawarc.h"
#include "rs_snapper.h"
#include "rs_dialogfactory.h"



RS_ActionDrawArc3P::RS_ActionDrawArc3P(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {
    reset();
}



RS_ActionDrawArc3P::~RS_ActionDrawArc3P() {}


RS_GuiAction* RS_ActionDrawArc3P::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawArc3P::getName(), 
            "arcs3p", 
            tr("&3 Points"));
        action->setStatusTip(tr("Draw arcs with 3 points"));
        action->setCommand("arc,a,a3");
        action->setKeycode("a3");
        action->setFactory(RS_ActionDrawArc3P::factory);
    }
    return action;
}


void RS_ActionDrawArc3P::reset() {
    data.reset();
    point1 = RS_Vector(false);
    point2 = RS_Vector(false);
    point3 = RS_Vector(false);
}



void RS_ActionDrawArc3P::init(int status) {
    RS_ActionBase::init(status);

    //reset();
}



RS_ActionInterface* RS_ActionDrawArc3P::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawArc3P(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawArc3P::trigger() {
    RS_ActionBase::trigger();

    preparePreview();
    if (data.isValid()) {
        RS_Arc* arc = new RS_Arc(container,
                                 data);
        arc->setLayerToActive();
        arc->setPenToActive();
        container->addEntity(arc);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(arc);
            document->endUndoCycle();
        }

        graphicView->createPainter();
        graphicView->drawEntity(arc);
        graphicView->moveRelativeZero(arc->getEndpoint());
        graphicView->restore();

        setStatus(SetPoint1);
        reset();
    } else {
        //RS_DIALOGFACTORY->requestWarningDialog(tr("Invalid arc data."));
        RS_DIALOGFACTORY->commandMessage(tr("Invalid arc data."));
    }
}



void RS_ActionDrawArc3P::preparePreview() {
    data.reset();
    if (point1.valid && point2.valid && point3.valid) {
        RS_Arc arc(NULL, data);
        bool suc = arc.createFrom3P(point1, point2, point3);
        if (suc) {
            data = arc.getData();
        }
    }
}


void RS_ActionDrawArc3P::mouseMoveEvent(RS_MouseEvent* e) {
    clearPreview();
    RS_Vector mouse = snapPoint(e);

    if (mouse.valid) {
        switch (getStatus()) {
        case SetPoint1:
            point1 = mouse;
            break;

        case SetPoint2:
            point2 = mouse;
            if (point1.valid) {
                RS_Line* line = new RS_Line(preview, RS_LineData(point1, point2));
                preview->addEntity(line);
            }
            break;

        case SetPoint3:
            point3 = mouse;
            preparePreview();
            if (data.isValid()) {
                preview->addEntity(new RS_Arc(preview, data));
            }
            break;

        default:
            break;
        }
    }
    drawPreview();
}



void RS_ActionDrawArc3P::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionDrawArc3P::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }
    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetPoint1:
        point1 = mouse;
        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        setStatus(SetPoint2);
        break;

    case SetPoint2:
        point2 = mouse;
        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        setStatus(SetPoint3);
        break;

    case SetPoint3:
        point3 = mouse;
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawArc3P::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    if (getStatus()==SetPoint1) {
        if (checkCommand("center", c)) {
            finish();
            graphicView->setCurrentAction(
                new RS_ActionDrawArc(*container, *graphicView));
        }
    }
}



RS_StringList RS_ActionDrawArc3P::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    return cmd;
}



void RS_ActionDrawArc3P::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPoint1:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify startpoint or [Center]"),
            tr("Cancel"));
        break;
    case SetPoint2:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify second point"), tr("Back"));
        break;
    case SetPoint3:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify endpoint"), tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawArc3P::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawArc3P::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarArcs);
    }
}

