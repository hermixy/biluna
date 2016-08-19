/****************************************************************************
** $Id: rs_actionpolylinedelbetween.cpp 8821 2008-02-02 17:31:21Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
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

#include "rs_actionpolylinedelbetween.h"
#include "rs_modification.h"



RS_ActionPolylineDelBetween::RS_ActionPolylineDelBetween(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                    container, graphicView) {

    reset();
}



RS_ActionPolylineDelBetween::~RS_ActionPolylineDelBetween() {}


RS_GuiAction* RS_ActionPolylineDelBetween::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineDelBetween::getName(), 
            "polylinedelbetween",
            tr("Delete Segments &Between Two Nodes"));
        action->setStatusTip(tr("Deletes all segments between two nodes in existing polylines"));
        action->setCommand("ob");
        action->setKeycode("ob");
        action->setFactory(RS_ActionPolylineDelBetween::factory);
    }

    return action;
}



void RS_ActionPolylineDelBetween::reset() {
    polyline = NULL;
    segment = NULL;
    node1 = RS_Vector(false);
    node2 = RS_Vector(false);
}


void RS_ActionPolylineDelBetween::init(int status) {
    RS_ActionBase::init(status);

    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
    reset();
}



RS_ActionInterface* RS_ActionPolylineDelBetween::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineDelBetween(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionPolylineDelBetween::trigger() {
    RS_ActionBase::trigger();

    if (polyline==NULL || polyline->isUndone() || segment==NULL ||
            node1.valid==false || node2.valid==false) {

        RS_DEBUG->print("RS_ActionPolylineDelBetween::trigger: "
                        "conditions not met", RS_Debug::D_WARNING);

        if (polyline!=NULL && polyline->isUndone()) {
            setStatus(SetPolyline);
            snapMode = RS2::ActionSnapFree;
            polyline=NULL;
        }
        return;
    }

    //deleteSnapper();
    //graphicView->moveRelativeZero(RS_Vector(0.0,0.0));
    graphicView->moveRelativeZero(node2);

    // delete nodes between node1 and node2 and handle undo functionality:
    RS_Modification m(*container, graphicView);
    polyline = m.deletePolylineNodesBetween(*polyline, *segment, node1, node2);

    //graphicView->moveRelativeZero(node2);

    if (polyline==NULL) {
        snapMode = RS2::ActionSnapFree;
        setStatus(SetPolyline);
    } else {
        snapMode = RS2::ActionSnapEndpoint;
        setStatus(SetNode2);
        //drawSnapper();
    }
}



void RS_ActionPolylineDelBetween::mouseMoveEvent(RS_MouseEvent* e) {
    // show snapper when specifying nodes:
    if ((getStatus()==SetNode1 || getStatus()==SetNode2) && polyline!=NULL) {
        clearPreview();
        snapPoint(e);
        drawPreview();
    }
}



void RS_ActionPolylineDelBetween::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineDelBetween::mouseReleaseEvent");

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        
        // specify polyline:
        case SetPolyline: {
                RS_Vector coord = graphicView->toGraph(e->x(), e->y());
                RS_Entity* entity = catchEntity(coord, RS2::ResolveNone);
                if (entity!=NULL && entity->rtti()==RS2::EntityPolyline) {
                    polyline = dynamic_cast<RS_Polyline*>(entity);
                    RS_Entity* sgm = catchEntity(coord, RS2::ResolveAll);
                    if (sgm!=NULL && sgm->isAtomic() && sgm->getParentContainer()==polyline) {
                        segment = dynamic_cast<RS_AtomicEntity*>(sgm);
                        snapMode = RS2::ActionSnapEndpoint;
                        setStatus(SetNode1);
                    }
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("No polyline found"));
                }
            }
            break;

        // specify nodes:
        case SetNode1:
        case SetNode2: {
                RS_CoordinateEvent ce(snapPoint(e));
                if (ce.isValid()) {
                    coordinateEvent(&ce);
                }
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        //deleteSnapper();
        graphicView->restore();
        init(getStatus()-1);
        snapMode = RS2::ActionSnapFree;
    }
}



void RS_ActionPolylineDelBetween::coordinateEvent(RS_CoordinateEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineDelBetween::coordinateEvent");

    if (e==NULL) {
        return;
    }

    RS_Vector coord = e->getCoordinate();

    switch (getStatus()) {

    // cannot specify entity with coordiantes:
    case SetPolyline:
        break;

    // set limiting node:
    case SetNode1:
        graphicView->moveRelativeZero(coord);
        node1 = coord;
        setStatus(SetNode2);
        break;

    // set limiting node:
    case SetNode2:
        graphicView->moveRelativeZero(coord);
        node2 = coord;
        trigger();
        setStatus(SetPolyline);
        break;

    default:
        break;
    }
}



void RS_ActionPolylineDelBetween::commandEvent(RS_CommandEvent* /*e*/) {
}



RS_StringList RS_ActionPolylineDelBetween::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetPolyline:
        break;
    case SetNode1:
        break;
    case SetNode2:
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionPolylineDelBetween::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPolyline:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify polyline"),
            tr("Cancel"));
        break;
    case SetNode1:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify first limiting node"),
            tr("Back"));
        break;
    case SetNode2:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify second limiting node"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionPolylineDelBetween::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionPolylineDelBetween::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

