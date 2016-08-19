/****************************************************************************
** $Id: rs_actiondrawlinerelangle.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawlinerelangle.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionDrawLineRelAngle::RS_ActionDrawLineRelAngle(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView,
    double angle,
    bool fixedAngle)
        :RS_ActionBase(
                           container, graphicView) {

    entity = NULL;
    this->angle = angle;
    this->fixedAngle = fixedAngle;
    length = 10.0;
    pos = RS_Vector(false);
}


RS_GuiAction* RS_ActionDrawLineRelAngle::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawLineRelAngle::getName(), 
            "linesrelativeangle",
            tr("R&elative Angle"));
        action->setStatusTip(tr("Draw line with relative angle"));
        action->setCommand("linerelangle,lr");
        action->setKeycode("lr");
        action->setFactory(RS_ActionDrawLineRelAngle::factory);
    }

    return action;
}

RS_ActionInterface* RS_ActionDrawLineRelAngle::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawLineRelAngle(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawLineRelAngle::trigger() {
    RS_ActionBase::trigger();

    clearPreview();

    RS_Creation creation(container, graphicView);
    creation.createLineRelAngle(pos,
                                entity,
                                angle,
                                length);

}



void RS_ActionDrawLineRelAngle::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineRelAngle::mouseMoveEvent begin");

    RS_Vector mouse(graphicView->toGraphX(e->x()),
                    graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetEntity:
        entity = catchEntity(e, RS2::ResolveAll);
        break;

    case SetPos:
        if (entity!=NULL) {
            clearPreview();

            RS_Entity* clone = entity->clone();
            clone->setHighlighted(true);
            preview->addPreviewEntity(clone);

            pos = snapPoint(e);

            RS_Creation creation(preview, NULL, false);
            creation.createLineRelAngle(pos,
                                        entity,
                                        angle,
                                        length);

            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionDrawLineRelAngle::mouseMoveEvent end");
}



void RS_ActionDrawLineRelAngle::mouseReleaseEvent(RS_MouseEvent* e) {

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetEntity: {
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL &&
                        (en->rtti()==RS2::EntityLine ||
                         en->rtti()==RS2::EntityArc ||
                         en->rtti()==RS2::EntityCircle)) {
                    entity = en;

                    setStatus(SetPos);
                }
            }
            break;

        case SetPos: {
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



void RS_ActionDrawLineRelAngle::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    switch (getStatus()) {
    case SetPos:
        pos = e->getCoordinate();
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawLineRelAngle::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetEntity:
    case SetPos:
        if (!fixedAngle && checkCommand("angle", c)) {
            clearPreview();
            graphicView->restore();
            setStatus(SetAngle);
        } else if (checkCommand("length", c)) {
            clearPreview();
            graphicView->restore();
            setStatus(SetLength);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                angle = RS_Math::deg2rad(a);
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(SetPos);
        }
        break;

    case SetLength: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                length = l;
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(SetPos);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawLineRelAngle::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetPos:
    case SetLength:
        if (!fixedAngle) {
            cmd += command("angle");
        }
        cmd += command("length");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawLineRelAngle::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetEntity:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Select base entity"),
                                                tr("Cancel"));
            break;
        case SetAngle:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter angle of line:"),
                                                tr("Back"));
            break;
        case SetLength:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter length of line:"),
                                                tr("Back"));
            break;
        case SetPos: {
            QString c;
            if (fixedAngle) {
                c = tr("length");
            }
            else {
                c = tr("angle/length");
            }
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify position [%1]").arg(c),
                tr("Back"));
            }
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawLineRelAngle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineRelAngle::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
        }
    }
}


