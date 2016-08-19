/****************************************************************************
** $Id: rs_actiondrawlinefree.cpp 9299 2008-03-03 14:12:57Z andrew $
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

#include "rs_actiondrawlinefree.h"

#include "rs_actionsetsnapmodefree.h"
#include "rs_snapper.h"
#include "rs_point.h"



RS_ActionDrawLineFree::RS_ActionDrawLineFree(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(
                    container, graphicView) {
    vertex = RS_Vector(false);
    polyline = NULL;
    RS_ActionSetSnapModeFree::factory();
}

RS_ActionDrawLineFree::~RS_ActionDrawLineFree() {
    if (polyline!=NULL) {
        delete polyline;
    }
}

RS_GuiAction* RS_ActionDrawLineFree::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawLineFree::getName(), 
            "linesfree",
                tr("&Freehand Line"));
        action->setStatusTip(tr("Draw freehand lines"));
        action->setCommand("linefree,freehand,lf");
        action->setKeycode("lf");
        action->setFactory(RS_ActionDrawLineFree::factory);
    }                                   
    return action;
}

RS_ActionInterface* RS_ActionDrawLineFree::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawLineFree(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawLineFree::trigger() {
    if (polyline!=NULL) {
        polyline->calculateBorders();
        container->addEntity(polyline);

        if (document) {
            document->startUndoCycle();
            document->addUndoable(polyline);
            document->endUndoCycle();
        }

        RS_DEBUG->print("RS_ActionDrawLineFree::trigger():"
                        " polyline added: %d", polyline->getId());
        
        clearPreview();
        graphicView->createPainter();
        graphicView->drawEntity(polyline);
        graphicView->restore();
        
        polyline = NULL;
    }
}



void RS_ActionDrawLineFree::mouseMoveEvent(RS_MouseEvent* e) {
    if (vertex.valid && polyline!=NULL) {
        clearPreview();

        RS_Vector v = snapPoint(e);
        if (v.valid) {
            RS_Entity* ent = polyline->addVertex(v);
            ent->setLayerToActive();
            ent->setPenToActive();

            preview->addEntity(polyline->clone());
    
            //graphicView->createPainter();
            //graphicView->drawEntity(ent);
            //graphicView->restore();
    
            vertex = v;
    
            RS_DEBUG->print("RS_ActionDrawLineFree::mouseMoveEvent():"
                            " line added: %d", ent->getId());
        }

        drawPreview();
    }
}



void RS_ActionDrawLineFree::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        vertex = snapPoint(e);
        polyline = new RS_Polyline(container,
                                   RS_PolylineData(vertex, vertex, 0));
        polyline->setLayerToActive();
        polyline->setPenToActive();
    }
    //else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton && !vertex.valid) {
    //}
}



void RS_ActionDrawLineFree::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        vertex = RS_Vector(false);
        trigger();
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        if (polyline!=NULL) {
            delete polyline;
            polyline = NULL;
        }
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionDrawLineFree::updateMouseButtonHints() {
    switch (getStatus()) {
    case 0:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Click and drag to draw a line"), tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawLineFree::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineFree::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
    }
}

