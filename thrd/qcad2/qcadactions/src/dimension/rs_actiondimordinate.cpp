/****************************************************************************
** $Id: rs_actiondimaligned.cpp 7800 2008-01-03 10:29:13Z andrew $
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

#include "rs_actiondimordinate.h"

#include "rs_snapper.h"
#include "rs_constructionline.h"
#include "rs_dialogfactory.h"



RS_ActionDimOrdinate::RS_ActionDimOrdinate(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionDimension(container, graphicView) {

    reset();
}



RS_ActionDimOrdinate::~RS_ActionDimOrdinate() {}

RS_GuiAction* RS_ActionDimOrdinate::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDimOrdinate::getName(), 
            "dimordinate",
            tr("&Ordinate"));
        action->setStatusTip(tr("Ordinate dimension"));
        action->setCommand("dimordinate,do");
        action->setKeycode("do");
        action->setFactory(RS_ActionDimOrdinate::factory);
    }

    return action;
}


RS_ActionInterface* RS_ActionDimOrdinate::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDimOrdinate(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionDimOrdinate::reset() {
    RS_ActionDimension::reset();

    edata = RS_DimOrdinateData(RS_Vector(false),
                               RS_Vector(false),
                               false);
    lastStatus = SetDefPoint1;
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true, true);
    }
}



void RS_ActionDimOrdinate::trigger() {
    RS_ActionDimension::trigger();

    //preparePreview();
    graphicView->moveRelativeZero(edata.defPoint1);

    RS_DimOrdinate* dim =
        new RS_DimOrdinate(container, data, edata);
    dim->setLayerToActive();
    dim->setPenToActive();
    dim->update();
    container->addEntity(dim);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(dim);
        document->endUndoCycle();
    }

    graphicView->createPainter();
    graphicView->drawEntity(dim);
    graphicView->restore();

    RS_DEBUG->print("RS_ActionDimOrdinate::trigger():"
                    " dim added: %d", dim->getId());
}



void RS_ActionDimOrdinate::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimOrdinate::mouseMoveEvent begin");

    clearPreview();
    RS_Vector mouse = snapPoint(e);

    if (mouse.valid) {
        switch (getStatus()) {
        case SetDefPoint1:
            break;

        case SetDefPoint2:
            if (edata.defPoint1.valid) {
                edata.defPoint2 = mouse;
                if (fabs(edata.defPoint2.x-edata.defPoint1.x) > 
                    fabs(edata.defPoint2.y-edata.defPoint1.y)) {
                    edata.xType = false;
                }
                else {
                    edata.xType = true;
                }

                RS_DimOrdinate* dim = new RS_DimOrdinate(preview, data, edata);
                dim->update();
                preview->addEntity(dim);
            }
            break;

        default:
            break;
        }
    }

    drawPreview();

    RS_DEBUG->print("RS_ActionDimOrdinate::mouseMoveEvent end");
}



void RS_ActionDimOrdinate::mouseReleaseEvent(RS_MouseEvent* e) {
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



void RS_ActionDimOrdinate::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector pos = e->getCoordinate();

    switch (getStatus()) {
    case SetDefPoint1:
        data.definitionPoint = RS_Vector(0,0);
        edata.defPoint1 = pos;
        graphicView->moveRelativeZero(pos);
        graphicView->restore();
        setStatus(SetDefPoint2);
        break;

    case SetDefPoint2:
        edata.defPoint2 = pos;
        /*
        graphicView->moveRelativeZero(pos);
        graphicView->restore();
        */
        trigger();
        reset();
        setStatus(SetDefPoint1);
        break;

    default:
        break;
    }
}



void RS_ActionDimOrdinate::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetText: {
            setText(c);
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(lastStatus);
            graphicView->enableCoordinateInput();
        }
        break;

    default:
        if (checkCommand("text", c)) {
            lastStatus = (Status)getStatus();
            graphicView->disableCoordinateInput();
            setStatus(SetText);
        }
        break;
    }
}



RS_StringList RS_ActionDimOrdinate::getAvailableCommands() {
    RS_StringList cmd = RS_ActionInterface::getAvailableCommands();

    switch (getStatus()) {
    case SetDefPoint1:
    case SetDefPoint2:
        cmd += command("text");
        break;

    default:
        break;
    }

    return cmd;
}



void RS_ActionDimOrdinate::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetDefPoint1:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify feature location"),
                tr("Cancel"));
            break;
        case SetDefPoint2:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify endpoint of leader"),
                tr("Back"));
            break;
        case SetText:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter dimension text:"), "");
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}

