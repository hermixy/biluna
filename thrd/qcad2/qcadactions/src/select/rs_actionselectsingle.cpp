/****************************************************************************
** $Id: rs_actionselectsingle.cpp 10350 2008-04-06 19:23:35Z andrew $
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

#include "rs_actionselectsingle.h"

#include "rs_snapper.h"
#include "rs_selection.h"



RS_ActionSelectSingle::RS_ActionSelectSingle(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        : RS_ActionSelectBase(container, graphicView) {

    en = NULL;
}


RS_GuiAction* RS_ActionSelectSingle::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionSelectSingle::getName(), 
            "selectsingle",
            tr("(De-)&Select Entity"));
        action->setStatusTip(tr("Selects single entities"));
        action->setCommand("selectsingle,ts");
        action->setKeycode("ts");
        action->setFactory(RS_ActionSelectSingle::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionSelectSingle::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSelectSingle(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionSelectSingle::trigger() {
    if (en!=NULL) {
        RS_Selection s(*container, graphicView);
        s.selectSingle(en);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    } else {
        RS_DEBUG->print("RS_ActionSelectSingle::trigger: Entity is NULL\n");
    }
}


void RS_ActionSelectSingle::keyPressEvent(RS_KeyEvent* e) {
    if (e->key()==Qt::Key_Enter) {
        finish();
    }
}


void RS_ActionSelectSingle::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        en = catchEntity(e);
        trigger();
    }
}



void RS_ActionSelectSingle::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::ArrowCursor);
}

