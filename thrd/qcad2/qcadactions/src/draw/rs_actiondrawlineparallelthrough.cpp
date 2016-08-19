/****************************************************************************
** $Id: rs_actiondrawlineparallelthrough.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawlineparallelthrough.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionDrawLineParallelThrough::RS_ActionDrawLineParallelThrough(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase( container, graphicView) {

    parallel = NULL;
    entity = NULL;
    distance = 1.0;
    number = 1;
    coord = RS_Vector(false);
}


RS_GuiAction* RS_ActionDrawLineParallelThrough::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawLineParallelThrough::getName(), 
            "linesparathrough",
                tr("Par&allel Through Point"));
        action->setStatusTip(tr("Draw parallel through a given point"));
        action->setKeycode("lg");
        action->setFactory(RS_ActionDrawLineParallelThrough::factory);
    }                                   
    return action;
}

RS_ActionInterface* RS_ActionDrawLineParallelThrough::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawLineParallelThrough(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawLineParallelThrough::trigger() {
    RS_ActionBase::trigger();

    if (entity!=NULL) {
        RS_Creation creation(container, graphicView);
        RS_Entity* e = creation.createParallelThrough(coord,
                       number,
                       entity);

        if (e==NULL) {
            RS_DEBUG->print("RS_ActionDrawLineParallelThrough::trigger:"
                            " No parallels added\n");
        }
    }
}



void RS_ActionDrawLineParallelThrough::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineParallelThrough::mouseMoveEvent begin");

    switch (getStatus()) {
    case SetEntity: {
            entity = catchEntity(e, RS2::ResolveAll);
        }
        break;

    case SetPos: 
        if (entity!=NULL) {
            clearPreview();
            RS_Entity* clone = entity->clone();
            clone->setHighlighted(true);
            preview->addPreviewEntity(clone);
            coord = snapPoint(e);
            if (coord.valid) {
                RS_Creation creation(preview, NULL, false);
                creation.createParallelThrough(coord,
                                               number,
                                               entity);
            }
            
            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionDrawLineParallelThrough::mouseMoveEvent end");
}



void RS_ActionDrawLineParallelThrough::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetEntity:
            entity = catchEntity(e, RS2::ResolveAll);
            if (entity!=NULL) {
                //entity->setHighlighted(true);
                //graphicView->drawEntity(entity);
                setStatus(SetPos);
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
        //if (entity!=NULL) {
            //entity->setHighlighted(false);
            //graphicView->drawEntity(entity);
            //entity=NULL;
        //}
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionDrawLineParallelThrough::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetPos:
        coord = mouse;
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawLineParallelThrough::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select entity"), tr("Cancel"));
        break;

    case SetPos:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify through point"),
                                            tr("Back"));
        break;

    case SetNumber:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Number:"), tr("Back"));
        break;

    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawLineParallelThrough::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetEntity:
    case SetPos: {
            if (checkCommand("number", c)) {
                deletePreview();
                clearPreview();
                lastStatus = (Status)getStatus();
                setStatus(SetNumber);
            }
        }
        break;

    case SetNumber: {
            bool ok;
            int n = c.toInt(&ok);
            if (ok==true) {
                if (n>0 && n<100) {
                    number = n;
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid number. "
                                                        "Try 1..99"));
                }
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawLineParallelThrough::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetEntity:
        cmd += command("number");
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionDrawLineParallelThrough::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineParallelThrough::updateToolBar() {
    if (getStatus()==SetPos && !isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
    }
}


// EOF
