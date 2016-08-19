/****************************************************************************
** $Id: rs_actionsetsnapmode.cpp 4408 2007-02-05 18:31:58Z andrew $
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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

#include "rs_actionreset.h"


/**
 * Constructor.
 */
RS_ActionReset::RS_ActionReset(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface( container, graphicView) {

}



RS_GuiAction* RS_ActionReset::createGuiAction() {
    static RS_GuiAction* action = NULL;
    
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionReset::getName(), 
            "reset", tr("&Selection pointer"));
        action->setKeycode("qq");
        action->setFactory(RS_ActionReset::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionReset::factory() {
    RS_DEBUG->print("RS_ActionReset::factory");
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionReset(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionReset::trigger() {
    RS_DEBUG->print("RS_ActionReset::trigger");
    graphicView->killAllActions();

    finish();

    graphicView->redraw();
}



void RS_ActionReset::init(int status) {
    RS_DEBUG->print("RS_ActionReset::init");
    RS_ActionInterface::init(status);
    trigger();
}

void RS_ActionReset::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
}
