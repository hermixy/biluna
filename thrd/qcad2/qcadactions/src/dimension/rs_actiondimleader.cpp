/****************************************************************************
** $Id: rs_actiondimleader.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actiondimleader.h"
#include "rs_snapper.h"



RS_ActionDimLeader::RS_ActionDimLeader(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    reset();
}



RS_ActionDimLeader::~RS_ActionDimLeader() {}


RS_GuiAction* RS_ActionDimLeader::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDimLeader::getName(), 
            "dimleader",
            tr("&Leader"));
        action->setStatusTip(tr("Leader (line sequence with arrow)"));
        action->setCommand("dimlea,de,ld");
        action->setKeycode("de,ld");
        action->setFactory(RS_ActionDimLeader::factory);
    }                                   

    return action;
}

RS_ActionInterface* RS_ActionDimLeader::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDimLeader(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionDimLeader::reset() {
    //data = RS_LineData(RS_Vector(false), RS_Vector(false));
    //start = RS_Vector(false);
    //history.clear();
    points.clear();
}



void RS_ActionDimLeader::init(int status) {
    RS_ActionBase::init(status);

    reset();
}



void RS_ActionDimLeader::trigger() {
    RS_ActionBase::trigger();

    if (points.count()>0) {

        RS_Leader* leader = new RS_Leader(container, RS_LeaderData(true));
        leader->setLayerToActive();
        leader->setPenToActive();

        for (int i=0; i < points.size(); ++i) {
            leader->addVertex(points.at(i));
        }

        container->addEntity(leader);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(leader);
            document->endUndoCycle();
        }

        clearPreview();
        graphicView->createPainter();
        graphicView->drawEntity(leader);
        graphicView->restore();

        RS_DEBUG->print("RS_ActionDimLeader::trigger(): leader added: %d",
                        leader->getId());
    }
}



void RS_ActionDimLeader::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDimLeader::mouseMoveEvent begin");
    clearPreview();

    RS_Vector mouse = snapPoint(e);
    if (mouse.valid && getStatus()==SetEndpoint && !points.isEmpty()) {

        // fill in lines that were already set:
        RS_Vector last(false);
        for (int i=0; i < points.size(); ++i) {
            if (last.valid) {
                preview->addEntity(new RS_Line(preview,
                                               RS_LineData(last, points.at(i))));
            }
            last = points.at(i);
        }

        RS_Vector p = points.last();
        preview->addEntity(new RS_Line(preview,
                                       RS_LineData(p, mouse)));
    }
    drawPreview();

    RS_DEBUG->print("RS_ActionDimLeader::mouseMoveEvent end");
}



void RS_ActionDimLeader::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        if (getStatus()==SetEndpoint) {
            trigger();
            reset();
            setStatus(SetStartpoint);
        } else {
            clearPreview();
            graphicView->restore();
            init(getStatus()-1);
        }
    }
}



void RS_ActionDimLeader::keyPressEvent(RS_KeyEvent* e) {
    if (getStatus()==SetEndpoint && e->key()==Qt::Key_Enter) {
        trigger();
        reset();
        setStatus(SetStartpoint);
    }
}



void RS_ActionDimLeader::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetStartpoint:
        //data.startpoint = mouse;
        points.clear();
        points.append(mouse);
        //start = data.startpoint;
        setStatus(SetEndpoint);
        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        break;

    case SetEndpoint:
        //data.endpoint = mouse;
        points.append(mouse);
        //trigger();
        //data.startpoint = data.endpoint;
        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        //graphicView->emulateMouseMoveEvent();
        break;

    default:
        break;
    }
}



void RS_ActionDimLeader::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    // enter to finish
    if (c=="") {
        trigger();
        reset();
        setStatus(SetStartpoint);
        //finish();
    }
}



RS_StringList RS_ActionDimLeader::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    return cmd;
}



void RS_ActionDimLeader::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetStartpoint:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
                                                tr("Cancel"));
            break;
        case SetEndpoint:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify next point"),
                                                tr("Finish"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDimLeader::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionDimLeader::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarDim);
        }
    }
}

