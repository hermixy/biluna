/****************************************************************************
** $Id: rs_actionzoomauto.cpp 2684 2005-09-07 00:42:52Z andrew $
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

#include "rs_actiondebugentities.h"


/**
 * Constructor.
 */
RS_ActionDebugEntities::RS_ActionDebugEntities(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface(container, graphicView) {

}


RS_GuiAction* RS_ActionDebugEntities::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDebugEntities::getName(), "&Debug Entities");
        action->setStatusTip("Prints debug info about entities");
        action->setCommand("debugentities,dp");
        action->setKeycode("dp");
        action->setFactory(RS_ActionDebugEntities::factory);
    }
    return action;
}



RS_ActionInterface* RS_ActionDebugEntities::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDebugEntities(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionDebugEntities::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



void RS_ActionDebugEntities::trigger() {
    RS_DEBUG->printEntity(RS_Debug::D_ERROR, container);
    finish();
}

