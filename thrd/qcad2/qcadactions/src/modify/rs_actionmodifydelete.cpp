/****************************************************************************
** $Id: rs_actionmodifydelete.cpp 10326 2008-04-05 19:53:18Z andrew $
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

#include "rs_actionmodifydelete.h"

#include "rs_actionselect.h"
#include "rs_modification.h"
#include "rs_snapper.h"
#include "rs_point.h"


RS_GuiAction* RS_ActionModifyDelete::action = NULL;


RS_ActionModifyDelete::RS_ActionModifyDelete(RS_EntityContainer& container,
        RS_GraphicView& graphicView) 
        :RS_ActionBase(container, graphicView) {}



RS_GuiAction* RS_ActionModifyDelete::createGuiAction() {
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyDelete::getName(), 
            "modifydelete",
            tr("&Delete"));
        action->setStatusTip(tr("Delete selected entities"));
        action->setCommand("delete,erase,er");
#ifdef __APPLE__
        action->setShortcut(Qt::Key_Backspace);
#else
        action->setShortcut(Qt::Key_Delete);
#endif
        action->setKeycode("er");
        action->setFactory(RS_ActionModifyDelete::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionModifyDelete::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a;
        if (container->countSelected()==0) {
            a = new RS_ActionSelect(*container, *graphicView, 
                RS_ActionModifyDelete::factory);
        }
        else {
            a = new RS_ActionModifyDelete(*container, *graphicView);
        }
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionModifyDelete::finish() {
    RS_ActionBase::finish();

}

void RS_ActionModifyDelete::init(int status) {
    RS_ActionBase::init(status);

    trigger();
}



void RS_ActionModifyDelete::trigger() {

    RS_DEBUG->print("RS_ActionModifyDelete::trigger()");

    RS_Modification m(*container, graphicView);
    m.remove();

    finish();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}



void RS_ActionModifyDelete::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
            //case Acknowledge:
            //    RS_DIALOGFACTORY->updateMouseWidget(tr("Acknowledge"),
            //    tr("Cancel"));
            //    break;
        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionModifyDelete::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::DelCursor);
}

void RS_ActionModifyDelete::updateToolBar() {
    switch (getStatus()) {
    default:
        //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
        break;
    }
}
