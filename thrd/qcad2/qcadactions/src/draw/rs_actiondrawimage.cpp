/****************************************************************************
** $Id: rs_actiondrawimage.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawimage.h"

#include "rs_creation.h"
#include "rs_modification.h"

/**
 * Constructor.
 */
RS_ActionDrawImage::RS_ActionDrawImage(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {}



RS_ActionDrawImage::~RS_ActionDrawImage() {}


RS_GuiAction* RS_ActionDrawImage::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawImage::getName(), 
            "menuimage", 
            tr("Insert &Image"));
        action->setStatusTip(tr("Insert raster image (bitmap)"));
        action->setCommand("image,im");
        action->setKeycode("im");
        action->setFactory(RS_ActionDrawImage::factory);
    }
    return action;
}

void RS_ActionDrawImage::init(int status) {
    RS_ActionBase::init(status);

    reset();

    data.file = RS_DIALOGFACTORY->requestImageOpenDialog();
    graphicView->redraw();

    if (!data.file.isEmpty()) {
        //std::cout << "file: " << data.file << "\n";

        img = RS_Img(data.file);
        setStatus(SetTargetPoint);
    } else {
        finish();
        updateToolBar();
        //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}



void RS_ActionDrawImage::reset() {
    data = RS_ImageData(0,
                        RS_Vector(0.0,0.0),
                        RS_Vector(1.0,0.0),
                        RS_Vector(0.0,1.0),
                        RS_Vector(1.0,1.0),
                        "",
                        50, 50, 0);
}



RS_ActionInterface* RS_ActionDrawImage::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawImage(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawImage::trigger() {
    clearPreview();

    if (!data.file.isEmpty()) {
        RS_Creation creation(container, graphicView);
        creation.createImage(data);
    }

    //graphicView->redraw();
    //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    finish();
    updateToolBar();
}


void RS_ActionDrawImage::mouseMoveEvent(RS_MouseEvent* e) {
    clearPreview();

    switch (getStatus()) {
    case SetTargetPoint: {
            data.insertionPoint = snapPoint(e);
            if (data.insertionPoint.valid) {
                //RS_Creation creation(preview, NULL, false);
                //creation.createInsert(data);
                RS_EntityContainer* rect = new RS_EntityContainer(preview);
                RS_Line* line;
                line = new RS_Line(rect,
                                   RS_LineData(RS_Vector(0, 0),
                                               RS_Vector(img.width(), 0)));
                line->setPen(RS_Preview::getPreviewPen());
                rect->addEntity(line);
                line = new RS_Line(rect,
                                   RS_LineData(RS_Vector(img.width(), 0),
                                               RS_Vector(img.width(), img.height())));
                line->setPen(RS_Preview::getPreviewPen());
                rect->addEntity(line);
                line = new RS_Line(rect,
                                   RS_LineData(RS_Vector(img.width(),
                                                         img.height()), RS_Vector(0, img.height())));
                line->setPen(RS_Preview::getPreviewPen());
                rect->addEntity(line);
                line = new RS_Line(rect,
                                   RS_LineData(RS_Vector(0, img.height()),
                                               RS_Vector(0, 0)));
                line->setPen(RS_Preview::getPreviewPen());
                rect->addEntity(line);
                rect->scale(RS_Vector(0,0),
                            RS_Vector(data.uVector.magnitude(), data.uVector.magnitude()));
                rect->rotate(RS_Vector(0,0), data.uVector.angle());
                rect->move(data.insertionPoint);
                preview->addEntity(rect);
            }
        }
        break;

    default:
        break;
    }
    drawPreview();
}



void RS_ActionDrawImage::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        finish();
    }
}



void RS_ActionDrawImage::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    data.insertionPoint = e->getCoordinate();
    trigger();
}



void RS_ActionDrawImage::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetTargetPoint:
        if (checkCommand("angle", c)) {
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetAngle);
        } else if (checkCommand("factor", c)) {
            deletePreview();
            clearPreview();
            lastStatus = (Status)getStatus();
            setStatus(SetFactor);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                setAngle(RS_Math::deg2rad(a));
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetFactor: {
            bool ok;
            double f = RS_Math::eval(c, &ok);
            if (ok==true) {
                setFactor(f);
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



RS_StringList RS_ActionDrawImage::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetTargetPoint:
        cmd += command("angle");
        cmd += command("factor");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawImage::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    case SetAngle:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter angle:"),
                                            "");
        break;
    case SetFactor:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter factor:"),
                                            "");
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawImage::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawImage::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}

