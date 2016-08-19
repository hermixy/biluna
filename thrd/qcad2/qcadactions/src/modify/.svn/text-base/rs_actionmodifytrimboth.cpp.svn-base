/****************************************************************************
** $Id: rs_actionmodifytrim.cpp 7924 2008-01-17 18:00:07Z andrew $
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

#include "rs_actionmodifytrimboth.h"


RS_GuiAction* RS_ActionModifyTrimBoth::action = NULL;


RS_ActionModifyTrimBoth::RS_ActionModifyTrimBoth(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        : RS_ActionModifyTrim(container, graphicView, true) {

}

RS_GuiAction* RS_ActionModifyTrimBoth::createGuiAction() {

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyTrimBoth::getName(), 
            "modifytrim2",
            tr("&Trim Both"));
        action->setStatusTip(tr("Trim or extend two entities to meet each other"));
        action->setCommand("trim2,extend2,tm");
        action->setKeycode("tm");
        action->setFactory(RS_ActionModifyTrimBoth::factory);
#ifdef RS_ARCH
        action->setCheckable(true);
#endif
    }

    return action;
}

RS_ActionInterface* RS_ActionModifyTrimBoth::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionModifyTrimBoth(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }

    return NULL;

}


void RS_ActionModifyTrimBoth::finish() {
    RS_ActionInterface::finish();

#ifdef RS_ARCH
    action->setChecked(false);
#endif
}
