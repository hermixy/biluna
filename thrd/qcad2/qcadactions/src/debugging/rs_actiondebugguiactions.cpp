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

#include "rs_actiondebugguiactions.h"


/**
 * Constructor.
 */
RS_ActionDebugGuiActions::RS_ActionDebugGuiActions(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface(container, graphicView) {

}


RS_GuiAction* RS_ActionDebugGuiActions::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDebugGuiActions::getName(), "&Debug Actions");
        action->setStatusTip("Prints debug info about all available actions");
        action->setCommand("debugactions,dg");
        action->setKeycode("dg");
        action->setFactory(RS_ActionDebugGuiActions::factory);
    }
    return action;
}



RS_ActionInterface* RS_ActionDebugGuiActions::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDebugGuiActions(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionDebugGuiActions::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



void RS_ActionDebugGuiActions::trigger() {
    RS_List<RS_GuiAction*> list = RS_GuiAction::getActions();

    for (int i=0; i<list.size(); ++i) {
        RS_GuiAction* action = list.at(i);
        RS_String iconId = action->getIconId();
        RS_String text = action->text();
        RS_StringList commandList = action->getCommandList();
        RS_StringList keycodeList = action->getKeycodeList();
        RS_StringList shortcutList = action->getShortcutList();

        RS_DEBUG->print(RS_Debug::D_ERROR,
            RS_String("%1\t%2\t%3\t%4\t%5")
            .arg(iconId)
            .arg(text)
            .arg(commandList.join(","))
            .arg(keycodeList.join(","))
            .arg(shortcutList.join(","))
        );
    }

    finish();
}

