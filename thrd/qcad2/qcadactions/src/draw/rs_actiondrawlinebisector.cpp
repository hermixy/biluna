/****************************************************************************
** $Id: rs_actiondrawlinebisector.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawlinebisector.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionDrawLineBisector::RS_ActionDrawLineBisector(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    bisector = NULL;
    length = 10.0;
    line1 = NULL;
    line2 = NULL;
    number = 1;
    coord1 = RS_Vector(false);
    coord2 = RS_Vector(false);
    lastStatus = SetLine1;
}

RS_GuiAction* RS_ActionDrawLineBisector::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawLineBisector::getName(), 
            "linesbisector",
                tr("&Bisector"));
        action->setStatusTip(tr("Draw bisectors"));
        action->setCommand("linebisector,bisector,lb");
        action->setKeycode("lb");
        action->setFactory(RS_ActionDrawLineBisector::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionDrawLineBisector::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawLineBisector(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawLineBisector::trigger() {
    RS_ActionBase::trigger();

    RS_Creation creation(container, graphicView);
    creation.createBisector(coord1,
                            coord2,
                            length,
                            number,
                            line1,
                            line2);
}



void RS_ActionDrawLineBisector::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineBisector::mouseMoveEvent begin");
                
    clearPreview();

    RS_Vector mouse = RS_Vector(graphicView->toGraphX(e->x()),
                                graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetLine1:
        break;

    case SetLine2: {
            coord2 = mouse;
            RS_Entity* en = catchEntity(e, RS2::ResolveAll);
            if (en!=NULL && en->rtti()==RS2::EntityLine) {
                line2 = dynamic_cast<RS_Line*>(en);


                RS_Creation creation(preview, NULL, false);
                creation.createBisector(coord1,
                                        coord2,
                                        length,
                                        number,
                                        line1,
                                        line2);
            }
        }
        break;

    default:
        break;
    }
                
    drawPreview();

    RS_DEBUG->print("RS_ActionDrawLineBisector::mouseMoveEvent end");
}



void RS_ActionDrawLineBisector::mouseReleaseEvent(RS_MouseEvent* e) {

    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deletePreview();
        clearPreview();
    graphicView->restore();

        init(getStatus()-1);
    } else {

        RS_Vector mouse = RS_Vector(graphicView->toGraphX(e->x()),
                                    graphicView->toGraphY(e->y()));

        switch (getStatus()) {
        case SetLine1: {
                coord1 = mouse;
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL && en->rtti()==RS2::EntityLine) {
                    line1 = dynamic_cast<RS_Line*>(en);
                }
            }
            setStatus(SetLine2);
            break;

        case SetLine2:
            coord2 = mouse;
            trigger();
            setStatus(SetLine1);
            break;
        }
    }

}


void RS_ActionDrawLineBisector::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
        lastStatus = (Status)getStatus();
        if (checkCommand("length", c)) {
            clearPreview();
            graphicView->restore();
            setStatus(SetLength);
        } else if (checkCommand("number", c)) {
            clearPreview();
            graphicView->restore();
            setStatus(SetNumber);
        }
        break;

    case SetLength: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                length = l;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetNumber: {
            bool ok;
            int n = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                number = n;
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



RS_StringList RS_ActionDrawLineBisector::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
        cmd += command("length");
        cmd += command("number");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawLineBisector::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetLine1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select first line"),
                                            tr("Cancel"));
        break;
    case SetLine2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select second line [length/number]"),
                                            tr("Back"));
        break;
    case SetLength:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter bisector length:"),
                                            tr("Back"));
        break;
    case SetNumber:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter number of bisectors:"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawLineBisector::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineBisector::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
}


