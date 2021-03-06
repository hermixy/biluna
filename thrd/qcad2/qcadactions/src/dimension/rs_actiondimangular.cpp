/****************************************************************************
** $Id: rs_actiondimangular.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondimangular.h"

#include "rs_creation.h"
#include "rs_information.h"
#include "rs_snapper.h"
#include "rs_dialogfactory.h"



RS_ActionDimAngular::RS_ActionDimAngular(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionDimension(container, graphicView) {
    reset();
}

RS_GuiAction* RS_ActionDimAngular::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDimAngular::getName(), 
            "dimangular",
            tr("&Angular"));
        action->setStatusTip(tr("Angular dimension"));
        action->setCommand("dimang,dn");
        action->setKeycode("dn");
        action->setFactory(RS_ActionDimAngular::factory);
    }                                   

    return action;
}


RS_ActionInterface* RS_ActionDimAngular::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDimAngular(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionDimAngular::reset() {
    RS_ActionDimension::reset();

    edata = RS_DimAngularData(RS_Vector(false),
                              RS_Vector(false),
                              RS_Vector(false),
                              RS_Vector(false));
    line1 = NULL;
    line2 = NULL;
    center = RS_Vector(false);
    RS_DIALOGFACTORY->requestOptions(this, true, true);
}



void RS_ActionDimAngular::trigger() {
    RS_ActionInterface::trigger();

    if (arc!=NULL || (line1!=NULL && line2!=NULL)) {
        RS_DimAngular* newEntity = NULL;

        newEntity = new RS_DimAngular(container,
                                      data,
                                      edata);

        newEntity->setLayerToActive();
        newEntity->setPenToActive();
        newEntity->update();
        container->addEntity(newEntity);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(newEntity);
            document->endUndoCycle();
        }
        clearPreview();
        graphicView->createPainter();
        graphicView->drawEntity(newEntity);
        graphicView->restore();

    } else {
        RS_DEBUG->print("RS_ActionDimAngular::trigger:"
                        " Entity is NULL\n");
    }
}



void RS_ActionDimAngular::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimAngular::mouseMoveEvent begin");

    clearPreview();
    RS_Vector mouse(graphicView->toGraphX(e->x()),
                    graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetLine1:
        break;

    case SetLine2:
        break;

    case SetPos:
        if (arc!=NULL || (line1!=NULL && line2!=NULL && center.valid)) {
            RS_Vector mouse = snapPoint(e);
            if (mouse.valid) {
                edata.definitionPoint4 = mouse;

                RS_DimAngular* d = new RS_DimAngular(preview, data, edata);
                d->update();

                preview->addEntity(d);
            }
        }
        break;

    default:
        break;
    }
    drawPreview();

    RS_DEBUG->print("RS_ActionDimAngular::mouseMoveEvent end");
}



void RS_ActionDimAngular::mouseReleaseEvent(RS_MouseEvent* e) {

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetLine1: {
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL) {
                    if (en->rtti()==RS2::EntityLine) {
                        line1 = dynamic_cast<RS_Line*>(en);
                        setStatus(SetLine2);
                    }
                    else if (en->rtti()==RS2::EntityArc) {
                        arc = dynamic_cast<RS_Arc*>(en);
                        edata.definitionPoint1 = arc->getCenter();
                        edata.definitionPoint2 = arc->getStartpoint();
                        edata.definitionPoint3 = arc->getCenter();
                        data.definitionPoint = arc->getEndpoint();
                        setStatus(SetPos);
                    }
                }
            }
            break;

        case SetLine2: {
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL &&
                        en->rtti()==RS2::EntityLine) {
                    line2 = dynamic_cast<RS_Line*>(en);

                    RS_VectorList sol =
                        RS_Information::getIntersectionLineLine(line1, line2);

                    if (sol.get(0).valid) {
                        center = sol.get(0);

                        if (center.distanceTo(line1->getStartpoint()) <
                                center.distanceTo(line1->getEndpoint())) {
                            edata.definitionPoint1 = line1->getStartpoint();
                            edata.definitionPoint2 = line1->getEndpoint();
                        } else {
                            edata.definitionPoint1 = line1->getEndpoint();
                            edata.definitionPoint2 = line1->getStartpoint();
                        }

                        if (center.distanceTo(line2->getStartpoint()) <
                                center.distanceTo(line2->getEndpoint())) {
                            edata.definitionPoint3 = line2->getStartpoint();
                            data.definitionPoint = line2->getEndpoint();
                        } else {
                            edata.definitionPoint3 = line2->getEndpoint();
                            data.definitionPoint = line2->getStartpoint();
                        }
                        graphicView->moveRelativeZero(center);
                        graphicView->restore();
                        setStatus(SetPos);
                    }
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
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }

}



void RS_ActionDimAngular::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    switch (getStatus()) {
    case SetPos:
        edata.definitionPoint4 = e->getCoordinate();
        trigger();
        reset();
        setStatus(SetLine1);
        break;

    default:
        break;
    }
}


void RS_ActionDimAngular::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    // setting new text label:
    if (getStatus()==SetText) {
        setText(c);
        RS_DIALOGFACTORY->requestOptions(this, true, true);
        graphicView->enableCoordinateInput();
        setStatus(lastStatus);
        return;
    }

    // command: text
    if (checkCommand("text", c)) {
        lastStatus = (Status)getStatus();
        graphicView->disableCoordinateInput();
        setStatus(SetText);
    }
}



RS_StringList RS_ActionDimAngular::getAvailableCommands() {
    RS_StringList cmd = RS_ActionInterface::getAvailableCommands();

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
    case SetPos:
        cmd += command("text");
        break;

    default:
        break;
    }

    return cmd;
}



void RS_ActionDimAngular::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (getStatus()==SetPos) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarDim);
        }
    }
}


void RS_ActionDimAngular::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetLine1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select first line or arc"),
                                            tr("Cancel"));
        break;
    case SetLine2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select second line"),
                                            tr("Cancel"));
        break;
    case SetPos:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify dimension arc line location"), tr("Cancel"));
        break;
    case SetText:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter dimension text:"), "");
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}

