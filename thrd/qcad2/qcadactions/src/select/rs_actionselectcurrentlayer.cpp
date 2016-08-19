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

#include "rs_actionselectcurrentlayer.h"

#include "rs_selection.h"



RS_ActionSelectCurrentLayer::RS_ActionSelectCurrentLayer(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionSelectBase( container, graphicView) {
}


RS_GuiAction* RS_ActionSelectCurrentLayer::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionSelectCurrentLayer::getName(), 
            "selectlayer",
            tr("Select &Current Layer"));
        action->setStatusTip(tr("Selects the layer that is currently active"));
        action->setCommand("selectcurrentlayer,tr");
        action->setKeycode("tr");
        action->setFactory(RS_ActionSelectCurrentLayer::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionSelectCurrentLayer::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSelectCurrentLayer(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionSelectCurrentLayer::init(int status) {
    RS_ActionSelectBase::init(status);
    trigger();
    finish();
}



void RS_ActionSelectCurrentLayer::trigger() {
    RS_Selection s(*container, graphicView);
    s.selectCurrentLayer();
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
}

