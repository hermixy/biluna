/****************************************************************************
** $Id: rs_actionpolylinetrim.cpp 9377 2008-03-10 20:44:40Z andrew $
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

#include "rs_actionpolylinetrim.h"
#include "rs_modification.h"



RS_ActionPolylineTrim::RS_ActionPolylineTrim(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                    container, graphicView) {

    reset();
}



RS_ActionPolylineTrim::~RS_ActionPolylineTrim() {}


RS_GuiAction* RS_ActionPolylineTrim::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineTrim::getName(), 
            "polylinetrim",
            tr("&Trim Segments"));
        action->setStatusTip(
            tr("Trims two polyline segments and deletes all nodes in between"));
        action->setCommand("ot");
        action->setKeycode("ot");
        action->setFactory(RS_ActionPolylineTrim::factory);
    }

    return action;
}



void RS_ActionPolylineTrim::reset() {
    polyline = NULL;
    segment1 = NULL;
    segment2 = NULL;
}


void RS_ActionPolylineTrim::init(int status) {
    RS_ActionBase::init(status);

    reset();
}



RS_ActionInterface* RS_ActionPolylineTrim::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineTrim(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionPolylineTrim::trigger() {
    RS_ActionBase::trigger();

    if (polyline==NULL || polyline->isUndone() ||
            segment1==NULL || segment2==NULL) {

        RS_DEBUG->print("RS_ActionPolylineTrim::trigger: "
                        "conditions not met", RS_Debug::D_WARNING);

        if (polyline!=NULL && polyline->isUndone()) {
            setStatus(SetSegment1);
            reset();
        }
        return;
    }

    RS_Vector relZ = graphicView->getRelativeZero();
    graphicView->moveRelativeZero(RS_Vector(0.0,0.0));

    // trim and handle undo functionality:
    RS_Modification m(*container, graphicView);
    polyline = m.polylineTrim(*polyline, *segment1, *segment2);

    graphicView->moveRelativeZero(relZ);

    //if (polyline==NULL) {
        setStatus(SetSegment1);
    //} else {
        //drawSnapper();
        //setStatus(SetSegment2);
    //}
}



void RS_ActionPolylineTrim::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineTrim::mouseReleaseEvent");

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetSegment1: {
                RS_Vector coord = graphicView->toGraph(e->x(), e->y());
                RS_Entity* entity = catchEntity(coord, RS2::ResolveAll);

                if (entity==NULL) {
                    RS_DEBUG->print("RS_ActionPolylineTrim::mouseReleaseEvent: entity is NULL");
                }
                else {
                    if (!entity->isAtomic()) {
                        RS_DEBUG->print("RS_ActionPolylineTrim::mouseReleaseEvent: entity is not atomic");
                    }
                    else {
                        if (entity->getParentContainer()==NULL) {
                            RS_DEBUG->print("RS_ActionPolylineTrim::mouseReleaseEvent: parent is NULL");
                        }
                        else {
                            if (entity->getParentContainer()->rtti()!=RS2::EntityPolyline) {
                                RS_DEBUG->print("RS_ActionPolylineTrim::mouseReleaseEvent: parent not polyline");
                            }
                        }
                    }
                }

                if (entity!=NULL && entity->isAtomic() &&
                        entity->getParentContainer()!=NULL && 
                        entity->getParentContainer()->rtti()==RS2::EntityPolyline) {
                    polyline = (RS_Polyline*)entity->getParentContainer();
                    segment1 = dynamic_cast<RS_AtomicEntity*>(entity);
                    setStatus(SetSegment2);
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("No polyline segment found"));
                }
            }
            break;

        case SetSegment2: {
                RS_Vector coord = graphicView->toGraph(e->x(), e->y());
                RS_Entity* entity = catchEntity(coord, RS2::ResolveAll);
                if (entity!=NULL && entity->isAtomic() &&
                        entity->getParentContainer()!=NULL && 
                        entity->getParentContainer()->rtti()==RS2::EntityPolyline &&
                        entity->getParentContainer()==polyline) {
                    segment2 = dynamic_cast<RS_AtomicEntity*>(entity);
                    trigger();
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("No polyline segment found"));
                }
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    }
}



RS_StringList RS_ActionPolylineTrim::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetSegment1:
        break;
    case SetSegment2:
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionPolylineTrim::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetSegment1:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify first segment"),
            tr("Cancel"));
        break;
    case SetSegment2:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify second segment"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionPolylineTrim::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionPolylineTrim::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
}

