/****************************************************************************
** $Id: rs_actiondrawtext.cpp 9338 2008-03-05 15:20:45Z andrew $
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

#include "rs_actiondrawtext.h"

#include "rs_snapper.h"
#include "rs_graphicview.h"



RS_ActionDrawText::RS_ActionDrawText(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    text = NULL;
    pos = RS_Vector(false);
    textChanged = true;
}



RS_ActionDrawText::~RS_ActionDrawText() {
    if (text!=NULL) {
        delete text;
        text = NULL;
    }
}


RS_GuiAction* RS_ActionDrawText::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawText::getName(), 
            "menutext", 
            tr("&Text"));
        action->setStatusTip(tr("Draw text entities"));
        action->setCommand("text,te");
        action->setKeycode("te");
        action->setFactory(RS_ActionDrawText::factory);
    }                                   
    return action;
}


void RS_ActionDrawText::init(int status) {
    RS_ActionBase::init(status);
    if (RS_DIALOGFACTORY!=NULL) {

        switch (status) {
        case ShowDialog: {
                clearPreview();
                reset();

                RS_Text tmp(NULL, data);
                if (RS_DIALOGFACTORY->requestTextDialog(&tmp)) {
                    data = tmp.getData();
                    preparePreview();
                    preview->setVisible(false);

                    setStatus(SetPos);
                    showOptions();
                } else {
                    hideOptions();
                    finish();
                }
                graphicView->redraw();
            }
            break;

        case SetPos:
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            break;

        default:
            break;
        }
    }
}



void RS_ActionDrawText::reset() {
    data = RS_TextData(RS_Vector(0.0,0.0),
                       1.0, 100.0,
                       RS2::VAlignTop,
                       RS2::HAlignLeft,
                       RS2::LeftToRight,
                       RS2::Exact,
                       1.0,
                       data.text,
                       "standard",
                       0.0,
                       RS2::Update);
}



RS_ActionInterface* RS_ActionDrawText::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawText(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawText::trigger() {

    RS_DEBUG->print("RS_ActionDrawText::trigger()");

    if (pos.valid) {
        clearPreview();

        RS_Text* t = new RS_Text(container, data);
        t->update();
        container->addEntity(t);

        if (document) {
            document->startUndoCycle();
            document->addUndoable(t);
            document->endUndoCycle();
        }

        graphicView->createPainter();
        graphicView->drawEntity(t);
        graphicView->restore();

        textChanged = true;
        setStatus(SetPos);
    }
}


void RS_ActionDrawText::preparePreview() {
    //clearPreview();
    data.insertionPoint = RS_Vector(0.0,0.0);
    if (text!=NULL) {
        delete text;
        text = NULL;
    }
    text = new RS_Text(preview, data);
    text->update();
    //text->setVisible(false);
    //preview->addEntity(text);
    textChanged = false;
}


void RS_ActionDrawText::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawText::mouseMoveEvent begin");
        
    if (getStatus()==SetPos) {
        clearPreview();
        RS_Vector mouse = snapPoint(e);
        if (mouse.valid) {
            pos = mouse;

            if (textChanged) {
                preparePreview();
            }
            RS_Entity* t = text->clone();
            t->setLayerToActive();
            t->move(mouse);
            preview->addEntity(t);
            if (!preview->isVisible()) {
                preview->setVisible(true);
            }
            offset = RS_Vector(graphicView->toGuiDX(pos.x),
                               -graphicView->toGuiDY(pos.y));
        }
        drawPreview();
    }

    RS_DEBUG->print("RS_ActionDrawText::mouseMoveEvent end");
}



void RS_ActionDrawText::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        //init(getStatus()-1);
        finish();
    }
}



void RS_ActionDrawText::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case ShowDialog:
        break;

    case SetPos:
        data.insertionPoint = mouse;
        trigger();
        break;

    default:
        break;
    }
}



void RS_ActionDrawText::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetPos:
        if (checkCommand("text", c)) {
            clearPreview();
            graphicView->disableCoordinateInput();
            graphicView->restore();
            setStatus(SetText);
        }
        break;

    case SetText: {
            setText(e->getCommand());
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            graphicView->enableCoordinateInput();
            setStatus(SetPos);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawText::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    if (getStatus()==SetPos) {
        cmd += command("text");
    }
    return cmd;
}



bool RS_ActionDrawText::optionsFromAction() {
    return true;
}



void RS_ActionDrawText::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetPos:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify insertion point"),
                                                tr("Cancel"));
            break;
        case SetText:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter text:"),
                                                tr("Back"));
            break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionDrawText::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawText::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetPos:
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
            break;
        default:
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
            break;
        }
    }
}



void RS_ActionDrawText::setText(const RS_String& t) {
    data.text = t;
    textChanged = true;
}



RS_String RS_ActionDrawText::getText() {
    return data.text;
}


void RS_ActionDrawText::setAngle(double a) {
    data.angle = a;
    textChanged = true;
}

double RS_ActionDrawText::getAngle() {
    return data.angle;
}

