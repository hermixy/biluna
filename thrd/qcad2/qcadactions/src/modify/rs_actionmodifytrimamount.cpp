/****************************************************************************
** $Id: rs_actionmodifytrimamount.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actionmodifytrimamount.h"

#include "rs_snapper.h"


RS_ActionModifyTrimAmount::RS_ActionModifyTrimAmount(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    trimEntity = NULL;
    trimCoord = RS_Vector(false);
    distance = 0.0;
}

RS_GuiAction* RS_ActionModifyTrimAmount::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyTrimAmount::getName(), 
            "modifytrimamount",
            tr("&Lengthen"));
        action->setStatusTip(tr("Lengthen / shorten by a given amount"));
        action->setCommand("lengthen,trimamount,le");
        action->setKeycode("le");
        action->setFactory(RS_ActionModifyTrimAmount::factory);
        
    }                          
    return action;
}


void RS_ActionModifyTrimAmount::init(int status) {
    RS_ActionBase::init(status);

    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
}

RS_ActionInterface* RS_ActionModifyTrimAmount::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyTrimAmount(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionModifyTrimAmount::trigger() {

    RS_DEBUG->print("RS_ActionModifyTrimAmount::trigger()");

    if (trimEntity!=NULL && trimEntity->isAtomic()) {

        RS_Modification m(*container, graphicView);
        m.trimAmount(trimCoord, dynamic_cast<RS_AtomicEntity*>(trimEntity), distance);

        trimEntity = NULL;
        setStatus(ChooseTrimEntity);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyTrimAmount::mouseReleaseEvent(RS_MouseEvent* e) {

    trimCoord = graphicView->toGraph(e->x(), e->y());
    trimEntity = catchEntity(e);

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case ChooseTrimEntity:
            if (trimEntity!=NULL && trimEntity->isAtomic()) {
                trigger();
            } else {
                if (trimEntity==NULL) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("No entity found. "));
                } else if (trimEntity->rtti()==RS2::EntityInsert) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("The chosen Entity is in a block. "
                           "Please edit the block."));
                } else {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("The chosen Entity is not an atomic entity "
                           "or cannot be trimmed."));
                }
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deleteSnapper();
        init(getStatus()-1);
    }
}



void RS_ActionModifyTrimAmount::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case ChooseTrimEntity: {
            bool ok;
            double d = RS_Math::eval(c, &ok);
            if (ok==true) {
                distance = d;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(ChooseTrimEntity);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionModifyTrimAmount::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case ChooseTrimEntity:
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionModifyTrimAmount::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseTrimEntity:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Select entity to trim or enter distance:"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyTrimAmount::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyTrimAmount::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
}


