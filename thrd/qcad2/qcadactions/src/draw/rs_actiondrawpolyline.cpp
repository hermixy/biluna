/****************************************************************************
** $Id: rs_actiondrawpolyline.cpp 9377 2008-03-10 20:44:40Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
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

#include "rs_actiondrawpolyline.h"
#include "rs_actionpolylineappend.h"
#include "rs_snapper.h"



RS_ActionDrawPolyline::RS_ActionDrawPolyline(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {

    reset();
    append = false;
}



RS_ActionDrawPolyline::~RS_ActionDrawPolyline() {}


RS_GuiAction* RS_ActionDrawPolyline::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawPolyline::getName(), 
            "polyline",
            tr("&Draw"));
        action->setStatusTip(tr("Draw polylines"));
        action->setCommand("polyline,pl");
        action->setKeycode("pl");
        action->setFactory(RS_ActionDrawPolyline::factory);
    }

    return action;
}



void RS_ActionDrawPolyline::reset() {
    polyline = NULL;
    //data = RS_PolylineData(RS_Vector(false), RS_Vector(false), false);
    start = RS_Vector(false);
    //history.clear();
    //bHistory.clear();
    nextArc = false;
    nextRadius = 0.0;
    point = RS_Vector(false);
}



void RS_ActionDrawPolyline::init(int status) {
    RS_ActionBase::init(status);

    //reset();
}

    
    
/**
 * Init this action for appending more segments.
 *
 * @param pl Existing polyline
 * @param sg Segment that was chosen (determines if the segments will be appended or prepended)
 */
void RS_ActionDrawPolyline::initForAppend(RS_Polyline* pl, RS_AtomicEntity* sg) {
    if (pl==NULL || sg==NULL) {
        RS_DEBUG->print(RS_Debug::D_WARNING, 
            "RS_ActionDrawPolyline::initForAppend: preconditions not met");
        return;
    }

    polyline = pl;

    int idx = pl->findEntity(sg);

    // prepend:
    if (idx<(int)pl->count()/2) {
        setStatus(SetPrevPoint);
        RS_Entity* first = polyline->firstEntity();
        if (first!=NULL && first->isAtomic()) {
            point = dynamic_cast<RS_AtomicEntity*>(first)->getStartpoint();
            start = polyline->getEndpoint();
            graphicView->moveRelativeZero(polyline->getStartpoint());
        }
    }

    // append:
    else {
        setStatus(SetNextPoint);
        RS_Entity* last = polyline->lastEntity();
        if (last!=NULL && last->isAtomic()) {
            point = dynamic_cast<RS_AtomicEntity*>(last)->getEndpoint();
            start = polyline->getStartpoint();
            graphicView->moveRelativeZero(polyline->getEndpoint());
        }
    }
    append = true;

    graphicView->emulateMouseMoveEvent();
    //graphicView->restore();
}



/**
 * Finishes this action and starts action for appending again.
 */
void RS_ActionDrawPolyline::backToAppend() {
    RS_ActionPolylineAppend* action = new RS_ActionPolylineAppend(*container,
                                             *graphicView);
    graphicView->setCurrentAction(action);
    finish();
}



RS_ActionInterface* RS_ActionDrawPolyline::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawPolyline(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawPolyline::trigger() {
    RS_ActionBase::trigger();

    if (polyline==NULL) {
        return;
    }

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(polyline);
        document->endUndoCycle();
    }

    polyline->calculateBorders();

    // upd view
    graphicView->createPainter();
    graphicView->drawEntity(polyline);
    if (getStatus()==SetNextPoint) {
        graphicView->moveRelativeZero(polyline->getEndpoint());
    }
    else {
        graphicView->moveRelativeZero(polyline->getStartpoint());
    }
    graphicView->restore();
    RS_DEBUG->print("RS_ActionDrawPolyline::trigger(): polyline added: %d",
                    polyline->getId());

    polyline = NULL;
    start = RS_Vector(false);
}



void RS_ActionDrawPolyline::mouseMoveEvent(RS_MouseEvent* e) {
    clearPreview();
    RS_Vector mouse = snapPoint(e);
    if (mouse.valid) {
        switch (getStatus()) {
        case SetNextPoint:
        case SetPrevPoint:
            if (point.valid) {
                bool isArc;
                RS_ArcData d = getNextArcData(mouse, &isArc);
        
                if (isArc) {
                    preview->addEntity(new RS_Arc(preview, d));
                } else {
                    preview->addEntity(new RS_Line(preview,
                                                  RS_LineData(point, mouse)));
                }
            }
            break;
        
        default:
            break;
        }
    }
    drawPreview();
}



void RS_ActionDrawPolyline::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        if (getStatus()==SetNextPoint || getStatus()==SetPrevPoint) {
            trigger();
            if (append==true) {
                backToAppend();
            }
        }
        clearPreview();
        graphicView->restore();
        history.clear();
        bHistory.clear();
        init(getStatus()-1);
    }
}



void RS_ActionDrawPolyline::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetStartpoint:
        point = mouse;
        history.append(mouse);
        bHistory.append(0.0);
        start = point;
        setStatus(SetNextPoint);
        graphicView->moveRelativeZero(mouse);
        graphicView->restore();
        updateMouseButtonHints();
        break;

    case SetNextPoint:
    case SetPrevPoint: {

            // find out bulge:
            RS_Vector endPoint = mouse;
            bool isArc;
            RS_ArcData d = getNextArcData(mouse, &isArc);
            double bulge;
            if (isArc) {
                RS_Arc arc(NULL, d);
                bulge = arc.getBulge();
                if (getStatus()==SetNextPoint) {
                    endPoint = arc.getEndpoint();
                }
                else {
                    endPoint = arc.getStartpoint();
                }
            } else {
                bulge = 0.0;
            }

            RS_DEBUG->print("RS_ActionDrawPolyline::coordinateEvent: "
                            "endPoint: %f/%f", endPoint.x,endPoint.y);

            // create polyline entity when the _second_ point is set:
            if (polyline==NULL) {
                polyline = new RS_Polyline(container, 
                    RS_PolylineData(RS_Vector(false), RS_Vector(false), false));
                polyline->addVertex(start, 0.0);
                polyline->setLayerToActive();
                polyline->setPenToActive();
                container->addEntity(polyline);
            }

            if (polyline!=NULL) {
                polyline->setNextBulge(bulge);
            }
            point = endPoint;

            history.append(endPoint);
            bHistory.append(bulge);

            if (polyline!=NULL) {
                polyline->addVertex(endPoint, 0.0, getStatus()==SetPrevPoint);
                clearPreview();
                graphicView->createPainter();
                graphicView->drawEntity(polyline);
                graphicView->moveRelativeZero(mouse);
                graphicView->restore();
            }
            setArc(false);
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            updateMouseButtonHints();
        }
        break;

    default:
        break;
    }
}



void RS_ActionDrawPolyline::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    switch (getStatus()) {
    case SetStartpoint:
        if (checkCommand("help", c)) {
            RS_DIALOGFACTORY->commandMessage(
                msgAvailableCommands()
                + getAvailableCommands().join(", "));
            return;
        }
        break;

    case SetNextPoint:
        if (checkCommand("close", c)) {
            close();
            updateMouseButtonHints();
            return;
        }

        if (checkCommand("undo", c)) {
            undo();
            updateMouseButtonHints();
            return;
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawPolyline::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetStartpoint:
        break;
    case SetNextPoint:
        if (history.count()>=2) {
            cmd += command("undo");
        }
        if (history.count()>=3) {
            cmd += command("close");
        }
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionDrawPolyline::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetStartpoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify first point"),
                                            tr("Cancel"));
        break;
    case SetNextPoint: {
            RS_String msg = "";

            if (history.count()>=3) {
                msg += command("close");
                msg += "/";
            }
            if (history.count()>=2) {
                msg += command("undo");
            }

            if (history.count()>=2) {
                RS_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify next point or [%1]").arg(msg),
                    tr("Back"));
            } else {
                RS_DIALOGFACTORY->updateMouseWidget(
                    tr("Specify next point"),
                    tr("Back"));
            }
        }
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}


void RS_ActionDrawPolyline::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}


void RS_ActionDrawPolyline::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

void RS_ActionDrawPolyline::close() {
    if (polyline!=NULL && polyline->count()>1 && start.valid) {
        nextArc=false;
        nextRadius=0.0;
        polyline->setClosed(true);
        polyline->endPolyline();
        trigger();
        setStatus(SetStartpoint);
        graphicView->moveRelativeZero(start);
        graphicView->restore();
        graphicView->emulateMouseMoveEvent();
    } else {
        RS_DIALOGFACTORY->commandMessage(
            tr("Cannot close sequence of lines: "
               "Not enough entities defined yet."));
    }
}

void RS_ActionDrawPolyline::undo() {
    if (history.count()>1) {
        history.removeLast();
        bHistory.removeLast();
        clearPreview();
        if (history.count()!=0) {
            point = history.last();
        }
        if (polyline!=NULL) {
            polyline->removeLastVertex();
            graphicView->moveRelativeZero(polyline->getEndpoint());
            graphicView->redraw();
        }
    } else {
        RS_DIALOGFACTORY->commandMessage(
            tr("Cannot undo: "
               "Not enough entities defined yet."));
    }
}


/**
 * Calculates the arc data for the next segment based on the last
 * polyline segment, the next vertex and the arc radius.
 */
RS_ArcData RS_ActionDrawPolyline::getNextArcData(const RS_Vector& nextPos, bool* ok) {
    RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge");

    // next segment is straight:
    if (nextArc==false || fabs(nextRadius)<1.0e-6) {
        RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge: no arc");
        if (ok!=NULL) {
            *ok = false;
        }
        return RS_ArcData();
    }


    // calculate end direction of last vertex. the arc segment will
    //   start in that direction
    double direction = 0.0;
    RS_Vector startPoint;
    if (polyline!=NULL) {
        // this might be the first entity:
        direction = 0.0;
        startPoint = start;

        if (getStatus()==SetNextPoint) {
            RS_Entity* last = polyline->lastEntity();
            if (last!=NULL && last->isAtomic()) {
                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(last);
                direction = RS_Math::correctAngle(ae->getDirection2() + M_PI);
                startPoint = ae->getEndpoint();
            }
        }
        else {
            RS_Entity* first = polyline->firstEntity();
            if (first!=NULL && first->isAtomic()) {
                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(first);
                direction = RS_Math::correctAngle(ae->getDirection1() + M_PI);
                startPoint = ae->getStartpoint();
            }
        }
    } else {
        direction = 0.0;
        startPoint = start;
    }

    RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge: direction: %f", direction);
    RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge: startPoint: %f/%f",
                    startPoint.x,startPoint.y);
    RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge: nextPos: %f/%f",
                    nextPos.x,nextPos.y);
    RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge: nextRadius: %f",
                    nextRadius);

    RS_Arc tmpArc(NULL, RS_ArcData());
    if (!tmpArc.createFrom2PDirectionRadius(startPoint, nextPos, direction, nextRadius)) {
        RS_DEBUG->print("RS_ActionDrawPolyline::getNextBulge: invalid data for bulge");
        if (ok!=NULL) {
            *ok = false;
        }
        return RS_ArcData();
    }
        
    if (getStatus()==SetPrevPoint) {
        tmpArc.reverse();
    }

    if (ok!=NULL) {
        *ok = true;
    }
    return tmpArc.getData();
}



/**
 * Sets that the next segment to be an arc (true) or a line (false).
 */
void RS_ActionDrawPolyline::setArc(bool on) {
    RS_DEBUG->print("RS_ActionDrawPolyline::setArc: %d", (int)on);
    nextArc = on;
    if (!nextArc) {
        //nextRadius = 0.0;
        if (polyline!=NULL) {
            polyline->setNextBulge(0.0);
        }
    }
}



/**
 * Sets the radius of the next polyline segment.
 */
void RS_ActionDrawPolyline::setRadius(double r) {
    nextRadius = r;
}


