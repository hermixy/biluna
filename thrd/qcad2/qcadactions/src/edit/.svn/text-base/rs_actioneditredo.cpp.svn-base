/****************************************************************************
** $Id: rs_actioneditundo.cpp 7167 2007-11-16 03:20:33Z michael $
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

#include "rs_actioneditredo.h"



/**
 * Constructor.
 *
 * @param undo true for undo and false for redo.
 */
RS_ActionEditRedo::RS_ActionEditRedo(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionEditUndo(container, graphicView, false) {
}



RS_ActionEditRedo::~RS_ActionEditRedo() {}


RS_GuiAction* RS_ActionEditRedo::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionEditRedo::getName(),  
            "redo", 
            tr("&Redo"));
        action->setStatusTip(tr("Redoes last action"));
        action->setCommand("redo,uu,r");
        action->setKeycode("uu");
        action->setShortcut("Ctrl+Shift+Z,Ctrl+Y");
        action->setFactory(RS_ActionEditRedo::factory);
    }

    return action;
}


RS_ActionInterface* RS_ActionEditRedo::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a =
            new RS_ActionEditRedo(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

