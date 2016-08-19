/****************************************************************************
** $Id: rs_actionzoomauto.cpp 3703 2006-11-28 17:50:03Z andrew $
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

#include "rs_actionviewgrid.h"

#include <QPixmap>

RS_GuiAction* RS_ActionViewGrid::action = NULL;

/**
 * Constructor.
 */
RS_ActionViewGrid::RS_ActionViewGrid(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface( container, graphicView) {
}


RS_GuiAction* RS_ActionViewGrid::createGuiAction() {
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionViewGrid::getName(), 
            
                "viewgrid",
            tr("&Grid"));
        action->setStatusTip(tr("Shows/hides the grid"));
        action->setCheckable(true);
        action->setChecked(true);
        action->setCommand("grid,gr");
        action->setShortcut("Ctrl+G");
        action->setKeycode("gr");
        action->setFactory(RS_ActionViewGrid::factory);
    }
    return action;
}


void RS_ActionViewGrid::init(int status) {
    RS_DEBUG->print("RS_ActionViewGrid::init");

    RS_ActionInterface::init(status);
    trigger();
}



RS_ActionInterface* RS_ActionViewGrid::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionViewGrid(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionViewGrid::trigger() {
    RS_DEBUG->print("RS_ActionViewGrid::trigger");
    
    graphic->setGridOn(!graphic->isGridOn());
    
    if (action!=NULL) {
        action->setChecked(graphic->isGridOn());
    }
    
    graphicView->redraw();
    finish();
}

