/****************************************************************************
** $Id: rs_actionsnapintersectionmanual.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionsnapintersectionmanual.h"

#include "rs_information.h"
#include "rs_snapper.h"


/**
 * @param both Trim both entities.
 */
RS_ActionSnapIntersectionManual::RS_ActionSnapIntersectionManual(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionInterface(
                           container, graphicView) {

    entity2 = NULL;
    entity1 = NULL;
    coord = RS_Vector(false);
}


RS_GuiAction* RS_ActionSnapIntersectionManual::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionSnapIntersectionManual::getName(), 
            
                "snapintersectionm", tr("I&ntersection Manually"));
        action->setStatusTip(tr("Snap to intersection points manually"));
        action->setCommand("sy");
        action->setKeycode("sy");
        action->setFactory(RS_ActionSnapIntersectionManual::factory);
    }                                   
    return action;
}


void RS_ActionSnapIntersectionManual::init(int status) {
    RS_ActionInterface::init(status);

    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
}



RS_ActionInterface* RS_ActionSnapIntersectionManual::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSnapIntersectionManual(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionSnapIntersectionManual::trigger() {

    RS_DEBUG->print("RS_ActionSnapIntersectionManual::trigger()");

    if (entity2!=NULL && entity2->isAtomic() &&
            entity1!=NULL && entity1->isAtomic()) {

        RS_VectorList sol =
            RS_Information::getIntersection(entity1, entity2, false);

        entity2 = NULL;
        entity1 = NULL;
        if (predecessor!=NULL) {
            RS_Vector ip = sol.getClosest(coord);

            if (ip.valid) {
                RS_CoordinateEvent ce(ip);
                if (ce.isValid()) {
                    predecessor->coordinateEvent(&ce);
                }
            }
        }
        finish();
    }
}



void RS_ActionSnapIntersectionManual::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionSnapIntersectionManual::mouseMoveEvent begin");

    RS_Entity* se = catchEntity(e);
    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());

    switch (getStatus()) {
    case ChooseEntity1:
        entity1 = se;
        break;

    case ChooseEntity2: {
            entity2 = se;
            coord = mouse;

            RS_VectorList sol =
                RS_Information::getIntersection(entity1, entity2, false);

            //for (int i=0; i<sol.getNumber(); i++) {
            //    ip = sol.get(i);
            //    break;
            //}

            RS_Vector ip = sol.getClosest(coord);

            if (ip.valid) {
                deletePreview();
                clearPreview();
                preview->addEntity(
                    new RS_Circle(preview,
                                  RS_CircleData(
                                      ip,
                                      graphicView->toGraphDX(4))));
                drawPreview();

                RS_DIALOGFACTORY->updateCoordinateWidget(ip,
                        ip - graphicView->getRelativeZero());

            }
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionSnapIntersectionManual::mouseMoveEvent end");
}



void RS_ActionSnapIntersectionManual::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {

        RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
        RS_Entity* se = catchEntity(e);

        switch (getStatus()) {
        case ChooseEntity1:
            entity1 = se;
            if (entity1!=NULL && entity1->isAtomic()) {
                setStatus(ChooseEntity2);
            }
            break;

        case ChooseEntity2:
            entity2 = se;
            coord = mouse;
            if (entity2!=NULL && entity2->isAtomic() && coord.valid) {
                trigger();
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



void RS_ActionSnapIntersectionManual::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseEntity1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select first entity"),
                                            tr("Back"));
        break;
    case ChooseEntity2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select second entity"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionSnapIntersectionManual::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionSnapIntersectionManual::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
}


// EOF
