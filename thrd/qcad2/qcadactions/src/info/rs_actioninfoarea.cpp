/****************************************************************************
** $Id: rs_actioninfoarea.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actioninfoarea.h"

#include "rs_snapper.h"



RS_ActionInfoArea::RS_ActionInfoArea(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {}


RS_GuiAction* RS_ActionInfoArea::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionInfoArea::getName(), 
            "infoarea",
            tr("&Polygonal Area"));
        action->setStatusTip(tr("Measures the area and circumference of a polygon"));
        action->setCommand("infoarea,area,ir");
        action->setKeycode("ir");
        action->setFactory(RS_ActionInfoArea::factory);
    }                                   
    return action;
}


void RS_ActionInfoArea::init(int status) {
    RS_ActionBase::init(status);

    //currentLine = NULL;
    //closingLine = NULL;

    //std::cout << "RS_ActionInfoArea::init: " << status << "\n";
}



RS_ActionInterface* RS_ActionInfoArea::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionInfoArea(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionInfoArea::trigger() {

    RS_DEBUG->print("RS_ActionInfoArea::trigger()");
    if (ia.isValid()) {
        ia.close();
        ia.calculate();
        double area = ia.getArea();
        double circ = ia.getCircumference();

        RS_DEBUG->print("RS_ActionInfoArea::trigger: area: %f", area);
        RS_DIALOGFACTORY->commandMessage(tr("Area: %1").arg(area));
        RS_DIALOGFACTORY->commandMessage(tr("Circumference: %1").arg(circ));
    }

    ia.reset();

    /*
    if (point1.valid && point2.valid) {
        double dist = point1.distanceTo(point2);
        QString str;
        str.sprintf("%.6f", dist);
        RS_DIALOGFACTORY->commandMessage(tr("Distance: %1").arg(str));
}
    */
}



void RS_ActionInfoArea::mouseMoveEvent(RS_MouseEvent* e) {
    //RS_DEBUG->print("RS_ActionInfoArea::mouseMoveEvent begin");

    if (getStatus()==SetFirstPoint ||
            getStatus()==SetNextPoint) {

        clearPreview();
        RS_Vector mouse = snapPoint(e);

        switch (getStatus()) {
        case SetFirstPoint:
            break;

        case SetNextPoint:
            if (ia.count()>1) {
                preview->addMeasuringArea(ia, *graphicView);
            }
            break;

        default:
            break;
        }

        drawPreview();
    }

    //RS_DEBUG->print("RS_ActionInfoArea::mouseMoveEvent end");
}



void RS_ActionInfoArea::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deletePreview();
        //clearPreview();

        // close the polygon (preview)
        //if (getStatus()==SetNextPoint) {
            clearPreview();
            graphicView->restore();

            /*
            if (currentLine!=NULL) {
                preview->removeEntity(currentLine);
                currentLine = NULL;
            }
            if (closingLine!=NULL) {
                preview->removeEntity(closingLine);
                closingLine = NULL;
            }

            currentLine = new RS_Line(preview,
                                      RS_LineData(prev,
                                                  point1));

            preview->addEntity(currentLine);

            drawPreview();
            */
        //}

        trigger();
        init(getStatus()-1);
    }
}



void RS_ActionInfoArea::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetFirstPoint:
        //point1 = mouse;

        clearPreview();

        ia.addPoint(mouse);
        RS_DIALOGFACTORY->commandMessage(tr("Point: %1/%2")
                                         .arg(mouse.x).arg(mouse.y));

        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        //prev = mouse;

        setStatus(SetNextPoint);
        break;

    case SetNextPoint:
        //if (point1.valid) {
            //point2 = mouse;
            /*deletePreview();
            clearPreview();
            */
            ia.addPoint(mouse);
            RS_DIALOGFACTORY->commandMessage(tr("Point: %1/%2")
                                             .arg(mouse.x).arg(mouse.y));

            //currentLine = NULL;
            clearPreview();
            preview->addMeasuringArea(ia, *graphicView);
            graphicView->moveRelativeZero(mouse);
            graphicView->restore();
            //prev = mouse;

            // automatically detect that the polyline is now closed
            if (ia.isClosed()) {
                trigger();
                setStatus(SetFirstPoint);
            }
            //trigger();
            //setStatus(SetFirstPoint);
        //}
        break;

    default:
        break;
    }
}


void RS_ActionInfoArea::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetFirstPoint:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify first point of polygon"),
            tr("Cancel"));
        break;
    case SetNextPoint:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify next point of polygon"),
            tr("Terminate"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionInfoArea::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionInfoArea::updateToolBar() {
    switch (getStatus()) {
    case SetFirstPoint:
    case SetNextPoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarInfo);
        break;
    }
}

