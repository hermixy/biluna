/****************************************************************************
** $Id: rs_actiondrawellipseaxis.cpp 9316 2008-03-03 19:49:12Z andrew $
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

#include "rs_actiondrawellipseaxis.h"

#include "rs_snapper.h"
#include "rs_constructionline.h"



/**
 * Constructor.
 *
 * @param isArc true if this action will produce an ellipse arc. 
 *              false if it will produce a full ellipse.
 */
RS_ActionDrawEllipseAxis::RS_ActionDrawEllipseAxis(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView,
    bool isArc)
        :RS_ActionBase(
                           container, graphicView) {

    this->isArc = isArc;
    center = RS_Vector(false);
    major = RS_Vector(false);
    ratio = 0.5;
    angle1 = 0.0;
    angle2 = 2*M_PI;
}



RS_ActionDrawEllipseAxis::~RS_ActionDrawEllipseAxis() {}



RS_GuiAction* RS_ActionDrawEllipseAxis::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawEllipseAxis::getName(),
            
                "ellipsesaxes",
            tr("&Ellipse (Axis)"));
        action->setStatusTip(tr("Ellipse with center and two points"));
        action->setCommand("ellipse,ep");
        action->setKeycode("ep");
        action->setFactory(RS_ActionDrawEllipseAxis::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionDrawEllipseAxis::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawEllipseAxis(*container, *graphicView, 
            false);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionDrawEllipseAxis::init(int status) {
    RS_ActionBase::init(status);

    if (status==SetCenter) {
        center = RS_Vector(false);
    }
    if (status<=SetMajor) {
        major = RS_Vector(false);
    }
    if (status<=SetMinor) {
        ratio = 0.5;
    }
    if (status<=SetAngle1) {
        angle1 = 0.0;
    }
    if (status<=SetAngle2) {
        angle2 = 2*M_PI;
    }
}



void RS_ActionDrawEllipseAxis::trigger() {
    RS_ActionBase::trigger();

    RS_EllipseData ellipseData(center, major,
                               ratio,
                               angle1, angle2,
                               false);
    RS_Ellipse* ellipse = new RS_Ellipse(container, ellipseData);
    ellipse->setLayerToActive();
    ellipse->setPenToActive();

    container->addEntity(ellipse);

    // upd. undo list:
    if (document!=NULL) {
        document->startUndoCycle();
        document->addUndoable(ellipse);
        document->endUndoCycle();
    }

    graphicView->createPainter();
    graphicView->drawEntity(ellipse);
    graphicView->restore();

    setStatus(SetCenter);

    RS_DEBUG->print("RS_ActionDrawEllipseAxis::trigger():"
                    " entity added: %d", ellipse->getId());
}



void RS_ActionDrawEllipseAxis::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawEllipseAxis::mouseMoveEvent begin");
    clearPreview();

    RS_Vector mouse = snapPoint(e);

    if (mouse.valid) {
        switch (getStatus()) {
        case SetCenter:
            break;

        case SetMajor:
            if (center.valid) {
                RS_EllipseData ed(center, mouse-center,
                                  0.5,
                                  0.0, 2*M_PI,
                                  false);
                preview->addEntity(new RS_Ellipse(preview, ed));
                    
                RS_Line* line = new RS_Line(
                    preview, 
                    RS_LineData(
                        center - (mouse-center),
                        center + (mouse-center)
                    )
                );
                line->setPen(RS_Preview::getPreviewPen());
                preview->addEntity(line);
            }
            break;

        case SetMinor:
            if (center.valid && major.valid) {
                /*
                RS_ConstructionLine line(NULL, 
                    RS_ConstructionLineData(center-major, center+major));
                double d = line.getDistanceToPoint(mouse);
                ratio = d/(major.magnitude()/2);
                */
                calculateRatio(mouse);

                RS_EllipseData ed(center, major,
                                  ratio,
                                  0.0, 2*M_PI,
                                  false);
                preview->addEntity(new RS_Ellipse(preview, ed));

                RS_Line* line1 = new RS_Line(
                    preview, 
                    RS_LineData(
                        center - major,
                        center + major
                    )
                );
                line1->setPen(RS_Preview::getPreviewPen());
                preview->addEntity(line1);
                
                RS_Vector offset;
                offset.setPolar(major.magnitude()*ratio, major.angle()+M_PI/2.0);
                
                RS_Entity* line2 = line1->clone();
                line2->move(offset);
                preview->addEntity(line2);
                
                RS_Entity* line3 = line1->clone();
                line3->move(-offset);
                preview->addEntity(line3);
                
                line1 = new RS_Line(
                    preview, 
                    RS_LineData(
                        center - offset,
                        center + offset
                    )
                );
                line1->setPen(RS_Preview::getPreviewPen());
                preview->addEntity(line1);
                
                offset.setPolar(major.magnitude(), major.angle());
                
                line2 = line1->clone();
                line2->move(offset);
                preview->addEntity(line2);
                
                line3 = line1->clone();
                line3->move(-offset);
                preview->addEntity(line3);
            }
            break;

        case SetAngle1:
            if (center.valid && major.valid) {
                RS_Vector m = mouse;
                m.rotate(center, -major.angle());
                RS_Vector v = m-center;
                v.scale(RS_Vector(1.0, 1.0/ratio));
                angle1 = v.angle(); // + major.angle();

                RS_Line* l = new RS_Line(preview, RS_LineData(center, mouse));
                l->setPen(preview->getPreviewPen());
                preview->addEntity(l);
                
                RS_EllipseData ed(center, major,
                                  ratio,
                                  angle1, angle1+1.0,
                                  false);
                preview->addEntity(new RS_Ellipse(preview, ed));
            }
            break;

        case SetAngle2:
            if (center.valid && major.valid) {
                RS_Vector m = mouse;
                m.rotate(center, -major.angle());
                RS_Vector v = m-center;
                v.scale(RS_Vector(1.0, 1.0/ratio));
                angle2 = v.angle(); // + major.angle();

                RS_Line* l = new RS_Line(preview, RS_LineData(center, mouse));
                l->setPen(preview->getPreviewPen());
                preview->addEntity(l);
                
                RS_EllipseData ed(
                    center, major,
                    ratio,
                    angle1, angle2,
                    false);
                preview->addEntity(new RS_Ellipse(preview, ed));
            }

        default:
            break;
        }
    }
            
    drawPreview();

    RS_DEBUG->print("RS_ActionDrawEllipseAxis::mouseMoveEvent end");
}



void RS_ActionDrawEllipseAxis::mouseReleaseEvent(RS_MouseEvent* e) {
    // Proceed to next status
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    }

    // Return to last status:
    else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}


void RS_ActionDrawEllipseAxis::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }
    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCenter:
        center = mouse;
        graphicView->moveRelativeZero(mouse);
        setStatus(SetMajor);
        break;

    case SetMajor:
        major = mouse-center;
        setStatus(SetMinor);
        break;

    case SetMinor: {
            /*
            RS_ConstructionLine line(NULL, 
                RS_ConstructionLineData(center-major, center+major));
            double d = line.getDistanceToPoint(mouse);
            ratio = d/(major.magnitude()/2);
            */
            calculateRatio(mouse);
            if (!isArc) {
                trigger();
                setStatus(SetCenter);
            } else {
                setStatus(SetAngle1);
            }
        }
        break;

    case SetAngle1: {
        //angle1 = center.angleTo(mouse);
        RS_Vector m = mouse;
        m.rotate(center, -major.angle());
        RS_Vector v = m-center;
        v.scale(RS_Vector(1.0, 1.0/ratio));
        angle1 = v.angle();
        setStatus(SetAngle2);
        } break;

    case SetAngle2: {
        //angle2 = center.angleTo(mouse);
        RS_Vector m = mouse;
        m.rotate(center, -major.angle());
        RS_Vector v = m-center;
        v.scale(RS_Vector(1.0, 1.0/ratio));
        angle2 = v.angle();
        trigger();
        } break;

    default:
        break;
    }
}



void RS_ActionDrawEllipseAxis::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetMinor: {
            bool ok;
            double m = RS_Math::eval(c, &ok);
            if (ok==true) {
                ratio = m / major.magnitude();
                if (!isArc) {
                    trigger();
                } else {
                    setStatus(SetAngle1);
                }
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
                angle1 = RS_Math::deg2rad(a);
                setStatus(SetAngle2);
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
        }
        break;

    case SetAngle2: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                angle2 = RS_Math::deg2rad(a);
                trigger();
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



RS_StringList RS_ActionDrawEllipseAxis::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    return cmd;
}



void RS_ActionDrawEllipseAxis::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetCenter:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify ellipse center"),
                                                tr("Cancel"));
            break;

        case SetMajor:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify endpoint of major axis"),
                                                tr("Back"));
            break;

        case SetMinor:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify endpoint or length of minor axis:"),
                tr("Back"));
            break;

        case SetAngle1:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify start angle"),
                                                tr("Back"));
            break;

        case SetAngle2:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify end angle"),
                                                tr("Back"));
            break;

        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawEllipseAxis::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawEllipseAxis::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarEllipses);
        }
    }
}

    

/**
 * Calculates the ellipse major / minor ratio. 'center' must be set,
 * 'major' must be set. 'point' is usually a mouse pointer coordinate.
 */
void RS_ActionDrawEllipseAxis::calculateRatio(const RS_Vector& point) {
    if (center.valid && major.valid && major.magnitude()>1.0e-6) {
        RS_ConstructionLine line(NULL, 
            RS_ConstructionLineData(center-major, center+major));
        double d = line.getDistanceToPoint(point);
        ratio = d/major.magnitude();
    }
    else {
        ratio = 0.5;
    }
}
