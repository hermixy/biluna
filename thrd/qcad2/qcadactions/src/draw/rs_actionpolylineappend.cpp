/****************************************************************************
** $Id: rs_actionpolylineappend.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionpolylineappend.h"
#include "rs_actiondrawpolyline.h"
#include "rs_modification.h"



RS_ActionPolylineAppend::RS_ActionPolylineAppend(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {

    reset();
}



RS_ActionPolylineAppend::~RS_ActionPolylineAppend() {}


RS_GuiAction* RS_ActionPolylineAppend::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineAppend::getName(), 
            "polylineappend",
            tr("A&ppend Node"));
        action->setStatusTip(tr("Appends nodes to existing polylines"));
        action->setCommand("op");
        action->setKeycode("op");
        action->setFactory(RS_ActionPolylineAppend::factory);
    }

    return action;
}



void RS_ActionPolylineAppend::reset() {
    polyline = NULL;
    segment = NULL;
}



void RS_ActionPolylineAppend::init(int status) {
    RS_ActionBase::init(status);

    reset();
}



RS_ActionInterface* RS_ActionPolylineAppend::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineAppend(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionPolylineAppend::trigger() {
    if (polyline==NULL || segment==NULL) {
        RS_DEBUG->print("RS_ActionPolylineAppend::trigger: preconditions not met");
        return;
    }
    RS_ActionBase::trigger();

    RS_ActionDrawPolyline* action = new RS_ActionDrawPolyline(*container,
                                             *graphicView);
    graphicView->setCurrentAction(action);
    action->initForAppend(polyline, segment);
    finish();
}



void RS_ActionPolylineAppend::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineAppend::mouseReleaseEvent");

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {

            // polyline segment specified:
        case SetPolylineSegment: {
                RS_Vector coord = graphicView->toGraph(e->x(), e->y());
                RS_Entity* entity = catchEntity(coord, RS2::ResolveAll);
                bool failed = false;
                if (entity!=NULL) {
                    if (entity->getParentContainer()!=NULL &&
                            entity->getParentContainer()->rtti()==RS2::EntityPolyline &&
                            entity->isAtomic()) {

                        if (((RS_Polyline*)entity->getParentContainer())->isClosed()) {
                            // cannot append..
                            RS_DIALOGFACTORY->commandMessage(
                                tr("Cannot append to closed polyline."));
                        }
                        else {
                            segment = dynamic_cast<RS_AtomicEntity*>(entity);
                            polyline = dynamic_cast<RS_Polyline*>(segment->getParentContainer());
                            trigger();
                        }
                    }
                    else {
                        failed = true;
                    }
                }
                else {
                    failed = true;
                }
                if (failed) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("No polyline entity found."));
                }
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deletePreview();
        //clearPreview();
        //deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionPolylineAppend::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetPolylineSegment:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify polyline segment"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionPolylineAppend::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionPolylineAppend::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

