/****************************************************************************
** $Id: rs_actioninfoangle.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actioninfoangle.h"

#include "rs_information.h"
#include "rs_snapper.h"



RS_ActionInfoAngle::RS_ActionInfoAngle(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionInterface(
                           container, graphicView) {}


RS_GuiAction* RS_ActionInfoAngle::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionInfoAngle::getName(), 
            "infoangle",
            tr("&Angle Between Two Lines"));
        action->setStatusTip(tr("Measures the angle between two lines"));
        action->setCommand("infoangle,ia");
        action->setKeycode("ia");
        action->setFactory(RS_ActionInfoAngle::factory);
    }                                   
    return action;
}


void RS_ActionInfoAngle::init(int status) {
    RS_ActionInterface::init(status);

    entity1 = NULL;
    entity2 = NULL;
    point1 = RS_Vector(false);
    point2 = RS_Vector(false);
    position = RS_Vector(false);
    reversed = false;
    angle = 0.0;
    noAngle = false;
}



RS_ActionInterface* RS_ActionInfoAngle::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionInfoAngle(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionInfoAngle::trigger() {

    RS_DEBUG->print("RS_ActionInfoAngle::trigger()");

    preparePreview();

    /*if (entity1!=NULL && entity2!=NULL) {
        RS_VectorList sol =
            RS_Information::getIntersection(entity1, entity2, false);

        if (sol.hasValid()) {
            intersection = sol.get(0);

            if (intersection.valid && point1.valid && point2.valid) {
                double angle1 = intersection.angleTo(point1);
                double angle2 = intersection.angleTo(point2);
                double angle = fabs(angle2-angle1);
                */

                if (noAngle==false) {
                    RS_String str;
                    str.sprintf("%.6f", RS_Math::rad2deg(angle));
                    RS_DIALOGFACTORY->commandMessage(tr("Angle: %1%2")
                                                     .arg(str).arg(QChar(0xB0)));
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("Lines are parallel"));
                }
            /*
            }
        } else {
            RS_DIALOGFACTORY->commandMessage(tr("Lines are parallel"));
        }
    }
    */
}

            
void RS_ActionInfoAngle::preparePreview() {
    if (entity1!=NULL && entity2!=NULL && point1.valid && point2.valid) {
        RS_VectorList sol =
            RS_Information::getIntersection(entity1, entity2, false);

        RS_DEBUG->print("RS_ActionInfoAngle::preparePreview: 001");

        if (sol.hasValid()) {
            RS_DEBUG->print("RS_ActionInfoAngle::preparePreview: 002");
            intersection = sol.get(0);

            if (intersection.valid) {
                double angle1 = intersection.angleTo(point1);
                double angle2 = intersection.angleTo(point2);
                angle = fabs(angle2-angle1);

                RS_DEBUG->print("RS_ActionInfoAngle::preparePreview: 003");
                preview->addMeasuringArc(intersection, 
                    point1, point2, position, *graphicView);
            }
            noAngle = false;
        }
        else {
            noAngle = true;
        }
    }
}


void RS_ActionInfoAngle::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionInfoAngle::mouseMoveEvent begin");
        
    RS_Vector mouse(graphicView->toGraphX(e->x()),
                    graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetEntity1:
        preparePreview();
        break;

    case SetEntity2:
        entity2 = catchEntity(e);
        if (entity2!=NULL && entity2!=entity1 && 
            entity2->rtti()==RS2::EntityLine) {

            point2 = entity2->getNearestPointOnEntity(mouse);
            position = mouse;
            
            clearPreview();
            preparePreview();
            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionInfoAngle::mouseMoveEvent end");
}



void RS_ActionInfoAngle::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {

        RS_Vector mouse(graphicView->toGraphX(e->x()),
                        graphicView->toGraphY(e->y()));

        switch (getStatus()) {
        case SetEntity1:
            entity1 = catchEntity(e);
            if (entity1!=NULL && entity1->rtti()==RS2::EntityLine) {
                point1 = entity1->getNearestPointOnEntity(mouse);
                setStatus(SetEntity2);
            }
            break;

        case SetEntity2:
            entity2 = catchEntity(e);
            if (entity2!=NULL && entity2->rtti()==RS2::EntityLine) {
                point2 = entity2->getNearestPointOnEntity(mouse);
                trigger();
                setStatus(SetEntity1);
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionInfoAngle::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity1:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify first line"),
            tr("Cancel"));
        break;
    case SetEntity2:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify second line"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionInfoAngle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionInfoAngle::updateToolBar() {
    switch (getStatus()) {
    case SetEntity1:
    case SetEntity2:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
        break;
    }
}


