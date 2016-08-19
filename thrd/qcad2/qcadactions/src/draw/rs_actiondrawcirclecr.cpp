/****************************************************************************
** $Id: rs_actiondrawcirclecr.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawcirclecr.h"
#include "rs_snapper.h"

/**
 * Constructor.
 */
RS_ActionDrawCircleCR::RS_ActionDrawCircleCR(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {

    reset();
}



RS_ActionDrawCircleCR::~RS_ActionDrawCircleCR() {}


RS_GuiAction* RS_ActionDrawCircleCR::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawCircleCR::getName(), 
            "circlescr",
            tr("Center, &Radius"));
        action->setStatusTip(tr("Draw circles with center and radius"));
        action->setCommand("circlecr,cr");
        action->setKeycode("cr");
        action->setFactory(RS_ActionDrawCircleCR::factory);
    }                                   
    return action;
}


void RS_ActionDrawCircleCR::reset() {
    data = RS_CircleData(RS_Vector(false), 0.0);
}



void RS_ActionDrawCircleCR::init(int status) {
    RS_ActionBase::init(status);
}



RS_ActionInterface* RS_ActionDrawCircleCR::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawCircleCR(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawCircleCR::trigger() {
    RS_ActionBase::trigger();

    RS_Circle* circle = new RS_Circle(container,
                                      data);
    circle->setLayerToActive();
    circle->setPenToActive();
    container->addEntity(circle);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(circle);
        document->endUndoCycle();
    }
    graphicView->createPainter();
    graphicView->drawEntity(circle);
    graphicView->moveRelativeZero(circle->getCenter());
    graphicView->restore();

    setStatus(SetCenter);

    RS_DEBUG->print("RS_ActionDrawCircleCR::trigger(): circle added: %d",
                    circle->getId());
}



void RS_ActionDrawCircleCR::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawCircleCR::mouseMoveEvent begin");
    clearPreview();

    RS_Vector mouse = snapPoint(e);
    if (mouse.valid) {
        switch (getStatus()) {
        case SetCenter:
            data.center = mouse;
            preview->addEntity(new RS_Circle(preview,
                                             data));
            break;
        }
    }
        
    drawPreview();

    RS_DEBUG->print("RS_ActionDrawCircleCR::mouseMoveEvent end");
}



void RS_ActionDrawCircleCR::mouseReleaseEvent(RS_MouseEvent* e) {
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



void RS_ActionDrawCircleCR::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawCircleCR::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetCenter:
        if (checkCommand("radius", c)) {
            setStatus(SetRadius);
        }
        break;

    case SetRadius: {
            bool ok;
            double r = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.radius = r;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(SetCenter);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawCircleCR::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetCenter:
        cmd += command("radius");
        break;
    default:
        break;
    }

    return cmd;
}

void RS_ActionDrawCircleCR::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetCenter:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify circle center"),
                                            tr("Cancel"));
        break;
    case SetRadius:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify circle radius"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawCircleCR::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawCircleCR::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarCircles);
    }
}

