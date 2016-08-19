/****************************************************************************
** $Id: rs_actioneditcopy.cpp 7804 2008-01-03 11:48:41Z andrew $
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

#include "rs_actioneditcutautoref.h"



RS_ActionEditCutAutoRef::RS_ActionEditCutAutoRef(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
    : RS_ActionEditCopy(container, graphicView, false, true) {
}



RS_ActionEditCutAutoRef::~RS_ActionEditCutAutoRef() {}



RS_GuiAction* RS_ActionEditCutAutoRef::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionEditCutAutoRef::getName(),
            
                "editcut",
            tr("Cu&t"));
        action->setStatusTip(tr("Cuts entities to the clipboard"));
        action->setShortcut("Ctrl+X");
        action->setCommand("cut,ct");
        action->setKeycode("ct");
        action->setFactory(RS_ActionEditCutAutoRef::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionEditCutAutoRef::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionEditCutAutoRef(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }

    return NULL;
}
