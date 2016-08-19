/****************************************************************************
** $Id: rs_actionmodifyrotate2.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actionmodifyrotate2.h"

#include "rs_snapper.h"
#include "rs_actionselect.h"



RS_ActionModifyRotate2::RS_ActionModifyRotate2(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {}


RS_GuiAction* RS_ActionModifyRotate2::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyRotate2::getName(), 
            "modifyrotate2",
            tr("Rotate T&wo"));
        action->setStatusTip(tr("Rotate entities around two centers"));
        action->setCommand("rotate2,r2");
        action->setKeycode("r2");
        action->setFactory(RS_ActionModifyRotate2::factory);
    }
    return action;
}

void RS_ActionModifyRotate2::init(int status) {
    RS_ActionBase::init(status);

}


RS_ActionInterface* RS_ActionModifyRotate2::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a;
        if (container->countSelected()==0) {
            a = new RS_ActionSelect(*container, *graphicView, 
                RS_ActionModifyRotate2::factory);
        }
        else {
            a = new RS_ActionModifyRotate2(*container, *graphicView);
        }
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionModifyRotate2::trigger() {

    RS_DEBUG->print("RS_ActionModifyRotate2::trigger()");

    RS_Modification m(*container, graphicView);
    m.rotate2(data);

    finish();

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyRotate2::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyRotate2::mouseMoveEvent begin");

    if (getStatus()==SetReferencePoint1 ||
            getStatus()==SetReferencePoint2) {

        clearPreview();
        RS_Vector mouse = snapPoint(e);
        if (mouse.valid) {
            switch (getStatus()) {
            case SetReferencePoint1:
                data.center1 = mouse;
                break;

            case SetReferencePoint2:
                if (data.center1.valid) {
                    data.center2 = mouse;
                }
                break;

            default:
                break;
            }
        }
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionModifyRotate2::mouseMoveEvent end");
}



void RS_ActionModifyRotate2::mouseReleaseEvent(RS_MouseEvent* e) {
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



void RS_ActionModifyRotate2::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint1:
        data.center1 = pos;
        setStatus(SetReferencePoint2);
        break;

    case SetReferencePoint2:
        data.center2 = pos;
        setStatus(ShowDialog);
        if (RS_DIALOGFACTORY->requestRotate2Dialog(data)) {
            trigger();
            //finish();
        }
        break;

    default:
        break;
    }
}


void RS_ActionModifyRotate2::commandEvent(RS_CommandEvent* /*e*/) {}



RS_StringList RS_ActionModifyRotate2::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    return cmd;
}




void RS_ActionModifyRotate2::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify absolute reference point"),
                                            tr("Cancel"));
        break;
    case SetReferencePoint2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify relative reference point"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyRotate2::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyRotate2::updateToolBar() {
    switch (getStatus()) {
    case SetReferencePoint1:
    case SetReferencePoint2:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}

