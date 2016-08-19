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

#include "rs_actioneditcut.h"

#include "rs_modification.h"
#include "rs_clipboard.h"

#include <QPixmap>

/**
 * Constructor.
 *
 * @param undo true for undo and false for redo.
 */
RS_ActionEditCut::RS_ActionEditCut(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        :RS_ActionEditCopy(container, graphicView, false, false) {
}



RS_ActionEditCut::~RS_ActionEditCut() {}



RS_GuiAction* RS_ActionEditCut::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionEditCut::getName(),
            
                "editcutref",
            tr("Cu&t with reference"));
        action->setStatusTip(tr("Cuts entities to the clipboard with a reference point"));
        action->setShortcut("Shift+Ctrl+X");
        action->setCommand("cutref,rt");
        action->setKeycode("rt");
        action->setFactory(RS_ActionEditCut::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionEditCut::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionEditCut(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }

    return NULL;
}
