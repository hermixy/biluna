/****************************************************************************
** $Id: rs_actionselectwindow.cpp 8348 2008-01-21 16:14:34Z andrew $
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

#include "rs_actiondeselectwindow.h"



RS_ActionDeselectWindow::RS_ActionDeselectWindow(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
    : RS_ActionSelectWindow(container, graphicView, false) {
}


RS_GuiAction* RS_ActionDeselectWindow::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDeselectWindow::getName(), 
            "deselectwindow",
            tr("Deselect &Window"));
        action->setStatusTip(tr("Deselects all entities in a given window"));
        action->setCommand("deselectwindow,uw");
        action->setKeycode("uw");
        action->setFactory(RS_ActionDeselectWindow::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionDeselectWindow::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDeselectWindow(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

