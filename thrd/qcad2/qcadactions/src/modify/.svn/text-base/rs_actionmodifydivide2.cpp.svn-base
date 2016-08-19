/****************************************************************************
** $Id: rs_actionmodifytrimamount.cpp 2802 2005-10-17 10:57:56Z andrew $
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

#include "rs_actionmodifydivide2.h"

#include "rs_snapper.h"


RS_GuiAction* RS_ActionModifyDivide2::action = NULL;


RS_ActionModifyDivide2::RS_ActionModifyDivide2(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
    :RS_ActionBase(container, graphicView) {

    divideEntity = NULL;
    cursor = RS_Vector(false);

    RS_SETTINGS->beginGroup("/Modify");
    remove = RS_SETTINGS->readNumEntry("/Divide2Remove", 1);
    RS_SETTINGS->endGroup();
}

RS_GuiAction* RS_ActionModifyDivide2::createGuiAction() {
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyDivide2::getName(), 
            "modifydivide2",
            tr("&Break out Segment"));
        action->setStatusTip(tr("Breaks out segments that are limited by intersections"));
        action->setFactory(RS_ActionModifyDivide2::factory);
        action->setCommand("divide2,break,d2");
        action->setKeycode("d2");
#ifdef RS_ARCH
        action->setCheckable(true);
#endif
    }                          

    return action;
}


RS_ActionInterface* RS_ActionModifyDivide2::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyDivide2(*container, *graphicView);
        graphicView->setCurrentAction(a);
#ifdef RS_ARCH
        action->setChecked(true);
#endif
        return a;
    }

    return NULL;
}



void RS_ActionModifyDivide2::finish() {
    RS_ActionBase::finish();

#ifdef RS_ARCH
    action->setChecked(false);
#endif
}


void RS_ActionModifyDivide2::init(int status) {
    RS_ActionBase::init(status);

    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
}



void RS_ActionModifyDivide2::trigger() {

    RS_DEBUG->print("RS_ActionModifyDivide2::trigger()");

    if (divideEntity!=NULL && divideEntity->isAtomic()) {
        RS_Modification m(*container, graphicView);
        m.divide2(dynamic_cast<RS_AtomicEntity*>(divideEntity), cursor, remove);

        divideEntity = NULL;
        setStatus(ChooseDivideEntity);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyDivide2::mouseReleaseEvent(RS_MouseEvent* e) {

    cursor = graphicView->toGraph(e->x(), e->y());
    divideEntity = catchEntity(e);

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case ChooseDivideEntity:
            if (divideEntity!=NULL && divideEntity->isAtomic()) {
                trigger();
            } else {
                if (divideEntity==NULL) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("No entity found."));
                } else if (divideEntity->rtti()==RS2::EntityInsert) {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("The chosen entity is in a block. "
                           "Please edit the block."));
                } else {
                    RS_DIALOGFACTORY->commandMessage(
                        tr("The chosen entity is not an atomic entity "
                           "or cannot be cut."));
                }
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    }
}



void RS_ActionModifyDivide2::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case ChooseDivideEntity: {
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionModifyDivide2::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case ChooseDivideEntity:
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionModifyDivide2::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseDivideEntity:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Select entity to break out segment"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyDivide2::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyDivide2::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
}


