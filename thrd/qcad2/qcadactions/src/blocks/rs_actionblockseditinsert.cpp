/****************************************************************************
** $Id: rs_actionblocksedit.cpp 4269 2007-01-20 18:51:56Z andrew $
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

#include "rs_actionblockseditinsert.h"

#include "rs_graphic.h"
#include "rs_insert.h"
#include "rs_dialogfactory.h"



RS_ActionBlocksEditInsert::RS_ActionBlocksEditInsert(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        : RS_ActionInterface(container, graphicView) {

    en = NULL;
}


RS_GuiAction* RS_ActionBlocksEditInsert::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionBlocksEditInsert::getName(), 
            
                "blockedit", 
            tr("&Edit Block from Insert"));
        action->setStatusTip(tr("Modifies the block that belongs to a chosen insert"));
        action->setCommand("bd");
        action->setKeycode("bd");
        action->setFactory(RS_ActionBlocksEditInsert::factory);
    }
    return action;
}


RS_ActionInterface* RS_ActionBlocksEditInsert::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionBlocksEditInsert(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionBlocksEditInsert::trigger() {
    RS_DEBUG->print("edit block");
    if (en==NULL || !en->isOfType(RS2::EntityInsert)) {
        RS_DIALOGFACTORY->commandMessage(
            tr("Entity is not an Insert."));
        setStatus(0);
        return;
    }

    if (graphic!=NULL) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->requestEditBlockWindow(dynamic_cast<RS_Insert*>(en)->getName());
        }
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_ActionBlocksEditInsert::trigger(): graphic is NULL");
    }
    //finish();
}



void RS_ActionBlocksEditInsert::init(int status) {
    RS_ActionInterface::init(status);
}



void RS_ActionBlocksEditInsert::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        en = catchEntity(e);
        trigger();
    }
}



void RS_ActionBlocksEditInsert::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::SelectCursor);
}
