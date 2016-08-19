/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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

#include "rs_actionmodifyisoproject.h"
#include "rs_actionselect.h"

#include "rs_snapper.h"



RS_ActionModifyIsoProject::RS_ActionModifyIsoProject(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {
    //data.offset = RS_Vector(0.0,0.0);
    data.segmentLength = 0.5;
    data.referencePoint = RS_Vector(0.0,0.0);
    data.targetPoint = RS_Vector(0.0,0.0);
    data.type = RS2::IsoSide;
    data.copy = true;
    data.useCurrentAttributes = false;
    data.useCurrentLayer = false;
}


RS_GuiAction* RS_ActionModifyIsoProject::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyIsoProject::getName(), 
            "modifyisometricprojection",
            tr("&Isometric Projection"));
        //action->setStatusTip(tr("Isometric Projection"));
        action->setCommand("isometric,pj");
        action->setKeycode("pj");
        action->setFactory(RS_ActionModifyIsoProject::factory);
    }

    return action;
}


RS_ActionInterface* RS_ActionModifyIsoProject::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a;
        if (container->countSelected()==0) {
            a = new RS_ActionSelect(*container, *graphicView, 
                RS_ActionModifyIsoProject::factory);
        }
        else {
            a = new RS_ActionModifyIsoProject(*container, *graphicView);
        }
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionModifyIsoProject::init(int status) {
    RS_ActionBase::init(status);
                    
}



void RS_ActionModifyIsoProject::trigger() {

    RS_DEBUG->print("RS_ActionModifyIsoProject::trigger()");

    data.copy = true;
    RS_Modification m(*container, graphicView);
    m.isoProject(data);

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyIsoProject::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyIsoProject::mouseMoveEvent begin");

    if (getStatus()==SetReferencePoint ||
            getStatus()==SetTargetPoint) {

        clearPreview();

        RS_Vector mouse = snapPoint(e);
        if (mouse.valid) {
            switch (getStatus()) {
            case SetReferencePoint:
                data.referencePoint = mouse;
                break;

            case SetTargetPoint:
                if (data.referencePoint.valid) {
                    data.targetPoint = mouse;

                    RS_EntityContainer* sub = new RS_EntityContainer(preview);
                    sub->setLayer(NULL);
                    sub->setSelected(true);
                    preview->addSelectionFrom(*container, sub);
                    preview->addEntity(sub);

                    data.copy = false;

                    RS_Modification m(*sub, NULL, false);
                    m.isoProject(data);
                    
                    //RS_DEBUG->printEntity(RS_Debug::D_ERROR, sub);
                }
                break;

            default:
                break;
            }
        }
        
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionModifyIsoProject::mouseMoveEvent end");
}



void RS_ActionModifyIsoProject::mouseReleaseEvent(RS_MouseEvent* e) {
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



void RS_ActionModifyIsoProject::coordinateEvent(RS_CoordinateEvent* e) {

    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint:
        data.referencePoint = pos;
        graphicView->moveRelativeZero(data.referencePoint);
        graphicView->restore();
        setStatus(SetTargetPoint);
        break;

    case SetTargetPoint:
        data.targetPoint = pos;
        graphicView->moveRelativeZero(data.targetPoint);
        setStatus(ShowDialog);
        //if (RS_DIALOGFACTORY->requestMoveDialog(data)) {
            //data.offset = targetPoint - referencePoint;
            //data.type = RS2::IsoSide;
            trigger();
            finish();
        //}
        break;

    default:
        break;
    }
}


void RS_ActionModifyIsoProject::updateMouseButtonHints() {
    switch (getStatus()) {
        /*case Select:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to move"),
                                           tr("Cancel"));
            break;*/
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
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



void RS_ActionModifyIsoProject::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyIsoProject::updateToolBar() {
    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        break;
    }
}

