/****************************************************************************
** $Id: rs_actionlibraryinsert.cpp 8484 2008-01-25 18:59:31Z andrew $
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

#include "rs_actionlibraryinsert.h"

#include "rs_creation.h"
#include "rs_modification.h"



/**
 * Constructor.
 */
RS_ActionLibraryInsert::RS_ActionLibraryInsert(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface(
                           container, graphicView) {}



RS_ActionLibraryInsert::~RS_ActionLibraryInsert() {}


RS_GuiAction* RS_ActionLibraryInsert::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionLibraryInsert::getName(), tr("&Insert Library Object"));
        action->setStatusTip(tr("Inserts an Object from the part library."));
        action->setFactory(RS_ActionLibraryInsert::factory);
    }                                   
    return action;
}


void RS_ActionLibraryInsert::init(int status) {
    RS_ActionInterface::init(status);

    reset();
}



void RS_ActionLibraryInsert::setFile(const RS_String& file) {
    data.file = file;

    if (!prev.open(file, RS2::FormatUnknown)) {
        RS_DIALOGFACTORY->commandMessage(tr("Cannot open file '%1'").arg(file));
    }
}


void RS_ActionLibraryInsert::reset() {
    /*data = RS_InsertData("",
                         RS_Vector(0.0,0.0),
                         RS_Vector(1.0,1.0),
                         0.0,
                         1, 1,
                         RS_Vector(1.0,1.0),
                         NULL,
                         RS2::Update);*/

    data.insertionPoint = RS_Vector(false);
    data.factor = 1.0;
    data.angle = 0.0;
}



RS_ActionInterface* RS_ActionLibraryInsert::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionLibraryInsert(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionLibraryInsert::trigger() {
    deletePreview();
    clearPreview();

    RS_Creation creation(container, graphicView);
    creation.createLibraryInsert(data);

    graphicView->redraw();
}


void RS_ActionLibraryInsert::mouseMoveEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case SetTargetPoint: {
        clearPreview();
        data.insertionPoint = snapPoint(e);

        if (data.insertionPoint.valid) {
            RS_EntityContainer* tmp = new RS_EntityContainer(preview);
            preview->addAllFrom(prev, tmp);
            tmp->move(data.insertionPoint);
            tmp->scale(data.insertionPoint,
                RS_Vector(data.factor, data.factor));
            // unit conversion:
            if (graphic!=NULL) {
                double uf = RS_Units::convert(1.0, prev.getUnit(),
                                              graphic->getUnit());
                tmp->scale(data.insertionPoint,
                               RS_Vector(uf, uf));
            }
            tmp->rotate(data.insertionPoint, data.angle);
            preview->addEntity(tmp);
        }
        drawPreview();
        } 
        break;

    default:
        break;
    }
}



void RS_ActionLibraryInsert::mouseReleaseEvent(RS_MouseEvent* e) {
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



void RS_ActionLibraryInsert::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    data.insertionPoint = e->getCoordinate();
    trigger();
}



void RS_ActionLibraryInsert::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetTargetPoint:
        if (checkCommand("angle", c)) {
            clearPreview();
            graphicView->restore();
            lastStatus = (Status)getStatus();
            setStatus(SetAngle);
        } else if (checkCommand("factor", c)) {
            clearPreview();
            graphicView->restore();
            lastStatus = (Status)getStatus();
            setStatus(SetFactor);
        }
        break;

    case SetAngle: {
            bool ok;
            double a = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.angle = RS_Math::deg2rad(a);
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



RS_StringList RS_ActionLibraryInsert::getAvailableCommands() {
    RS_StringList cmd = RS_ActionInterface::getAvailableCommands();

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



void RS_ActionLibraryInsert::updateMouseButtonHints() {
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



void RS_ActionLibraryInsert::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionLibraryInsert::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}

