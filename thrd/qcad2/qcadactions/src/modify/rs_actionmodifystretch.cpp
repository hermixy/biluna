/****************************************************************************
** $Id: rs_actionmodifystretch.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actionmodifystretch.h"

#include "rs_snapper.h"



RS_ActionModifyStretch::RS_ActionModifyStretch(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    firstCorner = RS_Vector(false);
    secondCorner = RS_Vector(false);
    referencePoint = RS_Vector(false);
    targetPoint = RS_Vector(false);
}

RS_GuiAction* RS_ActionModifyStretch::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyStretch::getName(), 
            "modifystretch",
            tr("&Stretch"));
        action->setStatusTip(tr("Stretch entities"));
        action->setCommand("stretch,ss");
        action->setKeycode("ss");
        action->setFactory(RS_ActionModifyStretch::factory);
    }                                   
    return action;
}


void RS_ActionModifyStretch::init(int status) {
    RS_ActionBase::init(status);

}


RS_ActionInterface* RS_ActionModifyStretch::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyStretch(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionModifyStretch::trigger() {

    RS_DEBUG->print("RS_ActionModifyStretch::trigger()");

    //clearPreview();

    RS_Modification m(*container, graphicView);
    m.stretch(firstCorner, secondCorner, targetPoint-referencePoint);

    setStatus(SetFirstCorner);

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyStretch::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyStretch::mouseMoveEvent begin");

    clearPreview();
    RS_Vector mouse = snapPoint(e);
    if (mouse.valid) {
        switch (getStatus()) {
        case SetFirstCorner:
            break;

        case SetSecondCorner:
            if (firstCorner.valid) {
                secondCorner = mouse;
                preview->addRectangle(firstCorner, secondCorner);
            }
            break;

        case SetReferencePoint:
            break;

        case SetTargetPoint:
            if (referencePoint.valid) {
                targetPoint = mouse;
                    
                RS_EntityContainer* sub = new RS_EntityContainer(preview);
                preview->addStretchablesFrom(*container, 
                    firstCorner, secondCorner, 
                    sub);
                sub->stretch(firstCorner, secondCorner, 
                    targetPoint-referencePoint);
                preview->addEntity(sub);
            }
            break;

        default:
            break;
        }
    }
    drawPreview();

    RS_DEBUG->print("RS_ActionModifyStretch::mouseMoveEvent end");
}



void RS_ActionModifyStretch::mouseReleaseEvent(RS_MouseEvent* e) {
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



void RS_ActionModifyStretch::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetFirstCorner:
        firstCorner = mouse;
        setStatus(SetSecondCorner);
        break;

    case SetSecondCorner:
        secondCorner = mouse;
        clearPreview();
        graphicView->restore();
        setStatus(SetReferencePoint);
        break;

    case SetReferencePoint:
        referencePoint = mouse;
        graphicView->moveRelativeZero(referencePoint);
        graphicView->restore();
        setStatus(SetTargetPoint);
        break;

    case SetTargetPoint:
        targetPoint = mouse;
        graphicView->moveRelativeZero(targetPoint);
        graphicView->restore();
        trigger();
        //finish();
        break;

    default:
        break;
    }

}


void RS_ActionModifyStretch::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetFirstCorner:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first corner"),
                                            tr("Cancel"));
        break;
    case SetSecondCorner:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify second corner"),
                                            tr("Back"));
        break;
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Back"));
        break;
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyStretch::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyStretch::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
    }
}

