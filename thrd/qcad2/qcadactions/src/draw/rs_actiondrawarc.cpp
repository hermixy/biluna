/****************************************************************************
** $Id: rs_actiondrawarc.cpp 9299 2008-03-03 14:12:57Z andrew $
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

#include "rs_actiondrawarc.h"

#include "rs_snapper.h"
#include "rs_dialogfactory.h"



RS_ActionDrawArc::RS_ActionDrawArc(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    reset();
}



RS_ActionDrawArc::~RS_ActionDrawArc() {}

RS_GuiAction* RS_ActionDrawArc::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawArc::getName(), 
            "arcscraa", 
            tr("&Center, Point, Angles"));
        action->setStatusTip(tr("Draw arcs"));
        action->setCommand("arcc,ar");
        action->setKeycode("ar");
        action->setFactory(RS_ActionDrawArc::factory);
    }

    return action;
}


void RS_ActionDrawArc::reset() {
    //bool rev = data.reversed;

    if (data.reversed) {
        data = RS_ArcData(RS_Vector(false),
                          0.0,
                          2*M_PI, 0.0,
                          true);
    } else {
        data = RS_ArcData(RS_Vector(false),
                          0.0,
                          0.0, 2*M_PI,
                          false);
    }
}



void RS_ActionDrawArc::init(int status) {
    RS_ActionBase::init(status);

    //reset();
}



RS_ActionInterface* RS_ActionDrawArc::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawArc(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawArc::trigger() {
    RS_ActionBase::trigger();

    RS_Arc* arc = new RS_Arc(container,
                             data);
    arc->setLayerToActive();
    arc->setPenToActive();
    container->addEntity(arc);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(arc);
        document->endUndoCycle();
    }

    graphicView->createPainter();
    graphicView->drawEntity(arc);
    graphicView->moveRelativeZero(arc->getCenter());
    graphicView->restore();

    setStatus(SetCenter);
    reset();

    RS_DEBUG->print("RS_ActionDrawArc::trigger(): arc added: %d",
                    arc->getId());
}



void RS_ActionDrawArc::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawArc::mouseMoveEvent begin");
            
    clearPreview();

    RS_Vector mouse = snapPoint(e);
    if (mouse.valid) {
        switch (getStatus()) {
        case SetCenter:
            data.center = mouse;
            break;
 
        case SetRadius:
            if (data.center.valid) {
                data.radius = data.center.distanceTo(mouse);
                preview->addEntity(new RS_Circle(preview,
                                                 RS_CircleData(data.center,
                                                               data.radius)));
            }
            break;
 
        case SetAngle1:
            data.angle1 = data.center.angleTo(mouse);
            if (data.reversed) {
                data.angle2 = RS_Math::correctAngle(data.angle1-M_PI/3);
            } else {
                data.angle2 = RS_Math::correctAngle(data.angle1+M_PI/3);
            }
            preview->addEntity(new RS_Arc(preview,
                                          data));
            break;
 
        case SetAngle2:
            data.angle2 = data.center.angleTo(mouse);
            preview->addEntity(new RS_Arc(preview,
                                          data));
            break;
 
        case SetIncAngle:
            data.angle2 = data.angle1 + data.center.angleTo(mouse);
            preview->addEntity(new RS_Arc(preview,
                                          data));
            break;
 
        case SetChordLength: {
                double x = data.center.distanceTo(mouse);
                if (fabs(x/(2*data.radius))<=1.0) {
                    data.angle2 = data.angle1 + asin(x/(2*data.radius)) * 2;
                    preview->addEntity(new RS_Arc(preview,
                                                  data));
                } 
            }
            break;
 
        default:
            break;
 
        }
    }
    drawPreview();

    RS_DEBUG->print("RS_ActionDrawArc::mouseMoveEvent end");
}



void RS_ActionDrawArc::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawArc::mouseReleaseEvent begin");
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
    RS_DEBUG->print("RS_ActionDrawArc::mouseReleaseEvent end");
}



void RS_ActionDrawArc::coordinateEvent(RS_CoordinateEvent* e) {
    RS_DEBUG->print("RS_ActionDrawArc::coordinateEvent begin");
    if (e==NULL) {
        return;
    }
    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        setStatus(SetRadius);
        break;

    case SetRadius:
        if (data.center.valid) {
            data.radius = data.center.distanceTo(mouse);
        }
        setStatus(SetAngle1);
        break;

    case SetAngle1:
        data.angle1 = data.center.angleTo(mouse);
        setStatus(SetAngle2);
        break;

    case SetAngle2:
        data.angle2 = data.center.angleTo(mouse);
        trigger();
        break;

    case SetIncAngle:
        data.angle2 = data.angle1 + data.center.angleTo(mouse);
        trigger();
        break;

    case SetChordLength: {
            double x = data.center.distanceTo(mouse);
            if (fabs(x/(2*data.radius))<=1.0) {
                data.angle2 = data.angle1 + asin(x/(2*data.radius)) * 2;
                trigger();
            }
        }
        break;

    default:
        break;
    }
    RS_DEBUG->print("RS_ActionDrawArc::coordinateEvent end");
}



void RS_ActionDrawArc::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    if (checkCommand("reversed", c)) {
        e->accept();
        setReversed(!isReversed());

        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestOptions(this, true, true);
        }
        return;
    }

    switch (getStatus()) {

    case SetRadius: {
            bool ok;
            double r = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.radius = r;
                setStatus(SetAngle1);
                e->accept();
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
        }
        break;

    case SetAngle1: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.angle1 = RS_Math::deg2rad(a);
                setStatus(SetAngle2);
                e->accept();
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
        }
        break;

    case SetAngle2: {
            if (checkCommand("angle", c)) {
                setStatus(SetIncAngle);
                e->accept();
            } else if (checkCommand("chord length", c)) {
                setStatus(SetChordLength);
                e->accept();
            } else {
                bool ok;
                double a = RS_Math::eval(c, &ok);
                if (ok==true) {
                    data.angle2 = RS_Math::deg2rad(a);
                    trigger();
                    e->accept();
                } else {
                    if (RS_DIALOGFACTORY!=NULL) {
                        RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                    }
                }
            }
        }
        break;

    case SetIncAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.angle2 = data.angle1 + RS_Math::deg2rad(a);
                trigger();
                e->accept();
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
        }
        break;

    case SetChordLength: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                if (fabs(l/(2*data.radius))<=1.0) {
                    data.angle2 = data.angle1 + asin(l/(2*data.radius)) * 2;
                    trigger();
                    e->accept();
                } else {
                    if (RS_DIALOGFACTORY!=NULL) {
                        RS_DIALOGFACTORY->commandMessage(
                            tr("Not a valid chord length"));
                    }
                }
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawArc::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    cmd += command("reversed");
    if (getStatus()==SetAngle2) {
        cmd += command("angle");
        cmd += command("chord");
    }
    return cmd;
}
    
    
    
bool RS_ActionDrawArc::checkCommand(const RS_String& cmd, const RS_String& str) {
    if (RS_ActionBase::checkCommand(cmd, str)) {
        return true;
    }

    RS_String lstr = str.toLower();

    if (cmd=="angle") {
        return (lstr=="angle" || lstr=="a");
    }
    else if (cmd=="chord length") {
        return (lstr=="chord" || lstr=="length" || lstr=="l");
    }

    return false;
}



void RS_ActionDrawArc::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetCenter:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify center"), tr("Cancel"));
            break;
        case SetRadius:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify radius"), tr("Back"));
            break;
        case SetAngle1:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify start angle:"), tr("Back"));
            break;
        case SetAngle2:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify end angle or [Angle/chord Length]"),
                tr("Back"));
            break;
        case SetIncAngle:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify included angle:"),
                                                tr("Back"));
            break;
        case SetChordLength:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify chord length:"),
                                                tr("Back"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawArc::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawArc::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarArcs);
        }
    }
}


