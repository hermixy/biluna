/****************************************************************************
** $Id: rs_actionblocksexplode.cpp 4269 2007-01-20 18:51:56Z andrew $
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

#include "rs_actionmodifydobooleanoperation.h"

#include "rs_modification.h"

/**
 * Constructor.
 */
RS_ActionModifyDoBooleanOperation::RS_ActionModifyDoBooleanOperation(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {}



RS_ActionModifyDoBooleanOperation::~RS_ActionModifyDoBooleanOperation() {}


RS_GuiAction* RS_ActionModifyDoBooleanOperation::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyDoBooleanOperation::getName(), 
            "modifytriangulate",
            tr("&Combine Hatches"));
        action->setStatusTip(tr("Combine multiple selected hatches into one"));
        //action->setKeycode("");
        action->setFactory(RS_ActionModifyDoBooleanOperation::factory);
    }

    return action;
}


void RS_ActionModifyDoBooleanOperation::init(int status) {
    RS_ActionBase::init(status);

    trigger();
}


RS_ActionInterface* RS_ActionModifyDoBooleanOperation::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyDoBooleanOperation(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionModifyDoBooleanOperation::trigger() {
    RS_Modification m(*container, graphicView);
    m.doBooleanOperation(RS_Modification::BoolOr);
    
    finish();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}


void RS_ActionModifyDoBooleanOperation::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (!isFinished()) {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        } else {
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        }
    }
}

