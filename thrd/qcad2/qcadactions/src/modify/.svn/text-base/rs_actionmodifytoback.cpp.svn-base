/****************************************************************************
** $Id: rs_actionmodifydelete.cpp 2691 2005-09-08 01:48:18Z andrew $
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

#include "rs_actionmodifytoback.h"

#include "rs_actionselectsingle.h"
#include "rs_modification.h"
#include "rs_snapper.h"



RS_ActionModifyToBack::RS_ActionModifyToBack(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView, bool toBack)
    :RS_ActionBase(container, graphicView) {
    this->toBack = toBack;
}



RS_GuiAction* RS_ActionModifyToBack::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyToBack::getName(), 
            "modifytoback",
            tr("Send to Bac&k"));
        action->setStatusTip(tr("Send entities to the back of other entities"));
        action->setCommand("toback,mb");
        action->setKeycode("mb");
        action->setFactory(RS_ActionModifyToBack::factory);
    }
    return action;
}


void RS_ActionModifyToBack::init(int status) {
    RS_ActionBase::init(status);

    trigger();
}


RS_ActionInterface* RS_ActionModifyToBack::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyToBack(*container, *graphicView, true);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionModifyToBack::trigger() {

    RS_DEBUG->print("RS_ActionModifyToBack::trigger()");

    RS_Modification m(*container, graphicView);
    m.moveToBack(toBack);

    finish();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyToBack::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateMouseWidget("", "");
    }
}



void RS_ActionModifyToBack::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::DelCursor);
}



void RS_ActionModifyToBack::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        }
    }
}

