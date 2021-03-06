/****************************************************************************
** $Id: rs_actionpolylinedel.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionpolylinedel.h"
#include "rs_modification.h"



RS_ActionPolylineDel::RS_ActionPolylineDel(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {

    reset();
}



RS_ActionPolylineDel::~RS_ActionPolylineDel() {}


RS_GuiAction* RS_ActionPolylineDel::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineDel::getName(), 
            "polylinedel",
            tr("&Delete Node"));
        action->setStatusTip(tr("Deletes nodes in existing polylines"));
        action->setCommand("od");
        action->setKeycode("od");
        action->setFactory(RS_ActionPolylineDel::factory);
    }

    return action;
}



void RS_ActionPolylineDel::reset() {
    polyline = NULL;
    node = RS_Vector(false);
}


void RS_ActionPolylineDel::init(int status) {
    RS_ActionBase::init(status);

    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
    reset();
}



RS_ActionInterface* RS_ActionPolylineDel::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineDel(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionPolylineDel::trigger() {
    RS_ActionBase::trigger();

    if (polyline==NULL || polyline->isUndone() || node.valid==false) {
        RS_DEBUG->print("RS_ActionPolylineDel::trigger: "
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
    graphicView->moveRelativeZero(node);

    // delete node and handle undo functionality:
    RS_Modification m(*container, graphicView);
    polyline = m.deletePolylineNode(*polyline, node);

    
    if (polyline==NULL) {
        snapMode = RS2::ActionSnapFree;
        setStatus(SetPolyline);
    }
    else {
        snapMode = RS2::ActionSnapEndpoint;
        setStatus(SetNode);
        //drawSnapper();
    }
}



void RS_ActionPolylineDel::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineDel::mouseMoveEvent begin");

    switch (getStatus()) {
    case SetPolyline:
        break;
        
    // show snapper when specifying node:
    case SetNode:
        if (polyline!=NULL) {
            clearPreview();
            snapPoint(e);
            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionPolylineDel::mouseMoveEvent end");
}



void RS_ActionPolylineDel::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineDel::mouseReleaseEvent");
    
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetPolyline: {
            RS_Vector coord = graphicView->toGraph(e->x(), e->y());
            RS_Entity* entity = catchEntity(coord, RS2::ResolveNone);
            if (entity!=NULL && entity->rtti()==RS2::EntityPolyline) {
                polyline = dynamic_cast<RS_Polyline*>(entity);
                snapMode = RS2::ActionSnapEndpoint;
                setStatus(SetNode);
            }
            else {
                RS_DIALOGFACTORY->commandMessage(tr("No polyline found"));
            }
            } break;

        case SetNode: {
            RS_CoordinateEvent ce(snapPoint(e));
            if (ce.isValid()) {
                coordinateEvent(&ce);
            }
        }

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deleteSnapper();
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
        snapMode = RS2::ActionSnapFree;
    }
}



void RS_ActionPolylineDel::coordinateEvent(RS_CoordinateEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineDel::coordinateEvent");
    
    if (e==NULL) {
        return;
    }

    RS_Vector coord = e->getCoordinate();

    switch (getStatus()) {

    // cannot specify polyline with coordinate input:
    case SetPolyline:
        break;

    // node specified:
    case SetNode:
        //graphicView->moveRelativeZero(coord);
        node = coord;
        trigger();
        updateMouseButtonHints();
        break;

    default:
        break;
    }
}



void RS_ActionPolylineDel::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    switch (getStatus()) {
    case SetPolyline:
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



RS_StringList RS_ActionPolylineDel::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetPolyline:
        break;
    case SetNode:
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionPolylineDel::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPolyline:
            RS_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify polyline"),
                    tr("Cancel"));
        break;
    case SetNode:
            RS_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify node to delete"),
                    tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionPolylineDel::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionPolylineDel::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

