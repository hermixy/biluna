/****************************************************************************
** $Id: rs_actiondimradial.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondimradial.h"

#include "rs_creation.h"
#include "rs_snapper.h"
#include "rs_dialogfactory.h"



RS_ActionDimRadial::RS_ActionDimRadial(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionDimension(container, graphicView) {
    reset();
}


RS_GuiAction* RS_ActionDimRadial::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDimRadial::getName(), 
            "dimradial",
            tr("&Radial"));
        action->setStatusTip(tr("Radial dimension"));
        action->setCommand("dimrad,dr");
        action->setKeycode("dr");
        action->setFactory(RS_ActionDimRadial::factory);
    }                                   

    return action;
}

RS_ActionInterface* RS_ActionDimRadial::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDimRadial(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionDimRadial::reset() {
    RS_ActionDimension::reset();

    edata = RS_DimRadialData(RS_Vector(false),
                             0.0);
    entity = NULL;
    pos = RS_Vector(false);
    RS_DIALOGFACTORY->requestOptions(this, true, true);
}



void RS_ActionDimRadial::trigger() {
    RS_ActionDimension::trigger();

    preparePreview();
    if (entity!=NULL) {
        RS_DimRadial* newEntity = NULL;

        newEntity = new RS_DimRadial(container,
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

    }
    else {
        RS_DEBUG->print("RS_ActionDimRadial::trigger:"
                        " Entity is NULL\n");
    }
}


void RS_ActionDimRadial::preparePreview() {
    if (entity!=NULL) {
        double angle = data.definitionPoint.angleTo(pos);
        double radius=0.0;
        if (entity->rtti()==RS2::EntityArc) {
            radius = dynamic_cast<RS_Arc*>(entity)->getRadius();
        } else if (entity->rtti()==RS2::EntityCircle) {
            radius = dynamic_cast<RS_Circle*>(entity)->getRadius();
        }

        edata.definitionPoint.setPolar(radius, angle);
        edata.definitionPoint += data.definitionPoint;
    }
}



void RS_ActionDimRadial::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimRadial::mouseMoveEvent begin");

    //RS_Vector mouse(graphicView->toGraphX(e->x()),
    //                graphicView->toGraphY(e->y()));
    clearPreview();

    switch (getStatus()) {
    case SetEntity:
        entity = catchEntity(e, RS2::ResolveAll);
        break;

    case SetPos:
        if (entity!=NULL) {
            pos = snapPoint(e);

            if (pos.valid) {
                preparePreview();

                RS_DimRadial* d = new RS_DimRadial(preview, data, edata);
                d->update();

                preview->addEntity(d);
            }
        }
        break;

    default:
        break;
    }
    drawPreview();

    RS_DEBUG->print("RS_ActionDimRadial::mouseMoveEvent end");
}



void RS_ActionDimRadial::mouseReleaseEvent(RS_MouseEvent* e) {

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetEntity: {
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL) {
                    if (en->rtti()==RS2::EntityArc ||
                            en->rtti()==RS2::EntityCircle) {
                        entity = en;
                        if (entity->rtti()==RS2::EntityArc) {
                            data.definitionPoint =
                                dynamic_cast<RS_Arc*>(entity)->getCenter();
                        } else if (entity->rtti()==RS2::EntityCircle) {
                            data.definitionPoint =
                                dynamic_cast<RS_Circle*>(entity)->getCenter();
                        }
                        graphicView->moveRelativeZero(data.definitionPoint);
                        graphicView->restore();
                        setStatus(SetPos);
                    } else {
                        RS_DIALOGFACTORY->commandMessage(tr("Not a circle "
                                                            "or arc entity"));
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

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionDimRadial::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    switch (getStatus()) {
    case SetPos:
        pos = e->getCoordinate();
        trigger();
        reset();
        setStatus(SetEntity);
        break;

    default:
        break;
    }
}



void RS_ActionDimRadial::commandEvent(RS_CommandEvent* e) {
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

    // setting angle
    if (getStatus()==SetPos) {
        bool ok;
        double a = RS_Math::eval(c, &ok);
        if (ok==true) {
            pos.setPolar(1.0, RS_Math::deg2rad(a));
            pos += data.definitionPoint;
            trigger();
            reset();
            setStatus(SetEntity);
        } else {
            RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
        }
        return;
    }
}



RS_StringList RS_ActionDimRadial::getAvailableCommands() {
    RS_StringList cmd = RS_ActionInterface::getAvailableCommands();

    switch (getStatus()) {
    case SetEntity:
    case SetPos:
        cmd += command("text");
        break;

    default:
        break;
    }

    return cmd;
}


void RS_ActionDimRadial::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select arc or circle entity"),
                                            tr("Cancel"));
        break;
    case SetPos:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Specify dimension line position or enter angle:"),
            tr("Cancel"));
        break;
    case SetText:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter dimension text:"), "");
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



