/****************************************************************************
**
** Copyright (C) 2001-2007 RibbonSoft. All rights reserved.
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

#include "rs_actionmodifydivide.h"

#include "rs_snapper.h"


RS_ActionModifyDivide::RS_ActionModifyDivide(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    divideEntity = NULL;
    divideCoord = RS_Vector(false);
}

RS_GuiAction* RS_ActionModifyDivide::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyDivide::getName(), 
            "modifydivide",
            tr("&Divide"));
        action->setStatusTip(tr("Divide entities"));
        action->setCommand("divide,di");
        action->setKeycode("di");
        action->setFactory(RS_ActionModifyDivide::factory);
    }                                   
    return action;
}



RS_ActionInterface* RS_ActionModifyDivide::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyDivide(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionModifyDivide::init(int status) {
    RS_ActionBase::init(status);
}



void RS_ActionModifyDivide::trigger() {

    RS_DEBUG->print("RS_ActionModifyDivide::trigger()");
    clearPreview();

    if (divideEntity!=NULL && divideEntity->isAtomic() && divideCoord.valid &&
            divideEntity->isPointOnEntity(divideCoord)) {

        RS_Modification m(*container, graphicView);
        m.divide(divideCoord, dynamic_cast<RS_AtomicEntity*>(divideEntity));

        divideEntity = NULL;
        divideCoord = RS_Vector(false);
        setStatus(ChooseDivideEntity);
    
        graphicView->redraw();

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyDivide::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyDivide::mouseMoveEvent begin");

    switch (getStatus()) {
    case ChooseDivideEntity:
        break;

    case SetDivideCoord:
        if (divideEntity!=NULL) {
            clearPreview();

            RS_Entity* clone = divideEntity->clone();
            clone->setHighlighted(true);
            preview->addPreviewEntity(clone);
            
            snapPoint(e);

            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionModifyTrim::mouseMoveEvent end");
}


void RS_ActionModifyDivide::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case ChooseDivideEntity:
            divideEntity = catchEntity(e);
            if (divideEntity==NULL) {
                RS_DIALOGFACTORY->commandMessage(tr("No Entity found."));
            } else if (divideEntity->rtti()!=RS2::EntityLine &&
                       divideEntity->rtti()!=RS2::EntityArc &&
                       divideEntity->rtti()!=RS2::EntityCircle &&
                       divideEntity->rtti()!=RS2::EntityEllipse) {

                RS_DIALOGFACTORY->commandMessage(
                    tr("Entity must be a line, arc, circle or ellipse."));
            } else {
                /*
                divideEntity->setHighlighted(true);
                graphicView->createPainter();
                graphicView->drawEntity(divideEntity);
                graphicView->restore();
                */
                setStatus(SetDivideCoord);
            }
            break;

        case SetDivideCoord:
            divideCoord = snapPoint(e);
            if (divideEntity==NULL) {
                RS_DIALOGFACTORY->commandMessage(tr("No Entity found."));
            } else if (!divideCoord.valid) {
                RS_DIALOGFACTORY->commandMessage(tr("Dividing point is invalid."));
            } else if (!divideEntity->isPointOnEntity(divideCoord)) {
                RS_DIALOGFACTORY->commandMessage(
                    tr("Dividing point is not on entity."));
            } else {
                clearPreview();
                trigger();
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        /*
        if (divideEntity!=NULL) {
            divideEntity->setHighlighted(false);
            graphicView->createPainter();
            graphicView->drawEntity(divideEntity);
        }
        */
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionModifyDivide::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseDivideEntity:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify entity to divide"),
                                            tr("Cancel"));
        break;
    case SetDivideCoord:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify dividing point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyDivide::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyDivide::updateToolBar() {

    switch (getStatus()) {
    case SetDivideCoord:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    case ChooseDivideEntity:
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


