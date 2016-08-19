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

#include "rs_actioneditcopyautoref.h"



RS_ActionEditCopyAutoRef::RS_ActionEditCopyAutoRef(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_ActionEditCopy(container, graphicView, true, true) {
}



RS_ActionEditCopyAutoRef::~RS_ActionEditCopyAutoRef() {}



RS_GuiAction* RS_ActionEditCopyAutoRef::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionEditCopyAutoRef::getName(),
            
                "editcopy",
            tr("&Copy"));
        action->setStatusTip(tr("Copies entities to the clipboard"));
        action->setShortcut("Ctrl+C");
        action->setCommand("copy,cp");
        action->setKeycode("cp");
        action->setFactory(RS_ActionEditCopyAutoRef::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionEditCopyAutoRef::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionEditCopyAutoRef(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }

    return NULL;
}
