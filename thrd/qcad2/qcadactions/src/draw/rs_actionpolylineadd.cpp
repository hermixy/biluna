/****************************************************************************
** $Id: rs_actionpolylineadd.cpp 9377 2008-03-10 20:44:40Z andrew $
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

#include "rs_actionpolylineadd.h"
#include "rs_modification.h"



RS_ActionPolylineAdd::RS_ActionPolylineAdd(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {

    reset();
}



RS_ActionPolylineAdd::~RS_ActionPolylineAdd() {}


RS_GuiAction* RS_ActionPolylineAdd::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineAdd::getName(), 
            "polylineadd",
            tr("&Add Node"));
        action->setStatusTip(tr("Adds nodes to existing polylines"));
        action->setCommand("oa");
        action->setKeycode("oa");
        action->setFactory(RS_ActionPolylineAdd::factory);
    }

    return action;
}



void RS_ActionPolylineAdd::reset() {
    polyline = NULL;
    segment = NULL;
    node = RS_Vector(false);
}



void RS_ActionPolylineAdd::init(int status) {
    RS_ActionBase::init(status);

    reset();
}



RS_ActionInterface* RS_ActionPolylineAdd::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineAdd(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionPolylineAdd::trigger() {
    RS_ActionBase::trigger();

    if (polyline==NULL || polyline->isUndone() || segment==NULL || node.valid==false) {
        RS_DEBUG->print("RS_ActionPolylineAdd::trigger: "
                        "conditions not met", RS_Debug::D_WARNING);
        if (polyline!=NULL && polyline->isUndone()) {
            setStatus(SetPolylineSegment);
            polyline=NULL;
        }
        return;
    }

    graphicView->moveRelativeZero(node);
    RS_Modification m(*container, graphicView);
    RS_Polyline* np = m.addPolylineNode(*polyline, *segment, node);

    if (np==NULL) {
        RS_DIALOGFACTORY->commandMessage(tr("Polyline was not modified"));
    }


    RS_DEBUG->print("RS_ActionPolylineAdd::trigger(): polyline node added to: %d",
                    polyline->getId());

    reset();
}



void RS_ActionPolylineAdd::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineAdd::mouseMoveEvent begin");

    if (getStatus()==SetNode && segment!=NULL) {
        clearPreview();
        RS_Vector mouse = snapPoint(e);
        if (mouse.valid) {
            RS_Line* l1 = new RS_Line(preview,
                                      RS_LineData(segment->getStartpoint(), mouse));
            l1->setPen(polyline->getPen(false));
            l1->setLayer(polyline->getLayer());
            RS_Line* l2 = new RS_Line(preview,
                                      RS_LineData(segment->getEndpoint(), mouse));
            l2->setPen(polyline->getPen(false));
            l2->setLayer(polyline->getLayer());

            preview->addEntity(l1);
            preview->addEntity(l2);
        }

        drawPreview();
    }

    RS_DEBUG->print("RS_ActionPolylineAdd::mouseMoveEvent end");
}



void RS_ActionPolylineAdd::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineAdd::mouseReleaseEvent");

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        
        // polyline segment specified:
        case SetPolylineSegment: {
                RS_Vector coord = graphicView->toGraph(e->x(), e->y());
                RS_Entity* entity = catchEntity(coord, RS2::ResolveAll);
                if (entity!=NULL) {
                    if (entity->getParentContainer()!=NULL &&
                            entity->getParentContainer()->rtti()==RS2::EntityPolyline &&
                            entity->isAtomic()) {

                        segment = dynamic_cast<RS_AtomicEntity*>(entity);
                        polyline = dynamic_cast<RS_Polyline*>(segment->getParentContainer());
                        setStatus(SetNode);
                        updateMouseButtonHints();
                    }
                } 
            }
            break;

        // node position specified:
        case SetNode: {
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
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionPolylineAdd::coordinateEvent(RS_CoordinateEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineAdd::coordinateEvent");

    if (e==NULL) {
        return;
    }

    RS_Vector coord = e->getCoordinate();

    switch (getStatus()) {

    // no coordinate input possible when specifying segment:
    case SetPolylineSegment:
        break;

    // node position specified:
    case SetNode:
        //graphicView->moveRelativeZero(coord);
        node = coord;
        trigger();
        updateMouseButtonHints();
        setStatus(SetPolylineSegment);
        break;

    default:
        break;
    }
}



void RS_ActionPolylineAdd::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    switch (getStatus()) {
    case SetPolylineSegment:
    case SetNode:
        if (checkCommand("help", c)) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
            return;
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionPolylineAdd::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetPolylineSegment:
        break;
    case SetNode:
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionPolylineAdd::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPolylineSegment:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify polyline segment"),
                                            tr("Cancel"));
        break;
    case SetNode:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify new node"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionPolylineAdd::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionPolylineAdd::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

