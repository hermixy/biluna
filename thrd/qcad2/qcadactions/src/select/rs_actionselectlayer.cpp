/****************************************************************************
** $Id: rs_actionselectlayer.cpp 9172 2008-02-23 15:25:20Z andrew $
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

#include "rs_actionselectlayer.h"

#include "rs_selection.h"



RS_ActionSelectLayer::RS_ActionSelectLayer(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionSelectBase( container, graphicView) {

    en = NULL;
}


RS_GuiAction* RS_ActionSelectLayer::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionSelectLayer::getName(), 
            "selectlayer",
            tr("(De-)Select La&yer"));
        action->setStatusTip(tr("(De-)Selects layers"));
        action->setCommand("selectlayer,tl");
        action->setKeycode("tl");
        action->setFactory(RS_ActionSelectLayer::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionSelectLayer::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSelectLayer(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionSelectLayer::trigger() {
    if (en!=NULL) {
        RS_Selection s(*container, graphicView);
        s.selectLayer(en);
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
        }
    } else {
        RS_DEBUG->print("RS_ActionSelectLayer::trigger: Entity is NULL\n");
    }
}



void RS_ActionSelectLayer::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        en = catchEntity(e);
        trigger();
    }
}



void RS_ActionSelectLayer::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::SelectCursor);
}

