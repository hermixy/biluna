/****************************************************************************
** $Id: rs_actionmodifymoverotate.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actionmodifymoverotate.h"

#include "rs_snapper.h"
#include "rs_actionselect.h"



RS_ActionModifyMoveRotate::RS_ActionModifyMoveRotate(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {
}

RS_GuiAction* RS_ActionModifyMoveRotate::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyMoveRotate::getName(),
            "modifymoverotate",
            tr("M&ove and Rotate"));
        action->setStatusTip(tr("Move and rotate entities"));
        action->setCommand("moverotate,mr");
        action->setKeycode("mr");
        action->setFactory(RS_ActionModifyMoveRotate::factory);
    }

    return action;
}



void RS_ActionModifyMoveRotate::init(int status) {
    RS_ActionBase::init(status);
}



RS_ActionInterface* RS_ActionModifyMoveRotate::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a;
        if (container->countSelected()==0) {
            a = new RS_ActionSelect(*container, *graphicView, 
                RS_ActionModifyMoveRotate::factory);
        }
        else {
            a = new RS_ActionModifyMoveRotate(*container, *graphicView);
        }
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionModifyMoveRotate::trigger() {

    RS_DEBUG->print("RS_ActionModifyMoveRotate::trigger()");

    RS_Modification m(*container, graphicView);
    m.moveRotate(data);

    finish();

    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionModifyMoveRotate::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyMoveRotate::mouseMoveEvent begin");

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
                    targetPoint = mouse;
                    data.offset = targetPoint-data.referencePoint;

                    RS_EntityContainer* sub = new RS_EntityContainer(preview);
                    preview->addSelectionFrom(*container, sub);
                    sub->rotate(data.referencePoint, data.angle);
                    sub->move(data.offset);

                    preview->addEntity(sub);
                }
                break;

            default:
                break;
            }
        }
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionModifyMoveRotate::mouseMoveEvent end");
}



void RS_ActionModifyMoveRotate::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        deletePreview();
        init(getStatus()-1);
    }
}



void RS_ActionModifyMoveRotate::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetReferencePoint:
        data.referencePoint = pos;
        setStatus(SetTargetPoint);
        break;

    case SetTargetPoint:
        targetPoint = pos;

        setStatus(ShowDialog);
        data.offset = targetPoint - data.referencePoint;
        if (RS_DIALOGFACTORY->requestMoveRotateDialog(data)) {
            trigger();
            //finish();
        }
        break;

    default:
        break;
    }
}


void RS_ActionModifyMoveRotate::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        if (checkCommand("angle", c)) {
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetAngle);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.angle = RS_Math::deg2rad(a);
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;
    }
}



RS_StringList RS_ActionModifyMoveRotate::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        cmd += command("angle");
        break;

    default:
        break;
    }

    return cmd;
}



void RS_ActionModifyMoveRotate::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
                                            tr("Back"));
        break;
    case SetAngle:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter rotation angle:"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyMoveRotate::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyMoveRotate::updateToolBar() {
    switch (getStatus()) {
    case SetReferencePoint:
    case SetTargetPoint:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}


// EOF
