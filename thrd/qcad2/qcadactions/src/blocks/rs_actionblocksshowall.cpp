/****************************************************************************
** $Id: rs_actionblocksshowall.cpp 9172 2008-02-23 15:25:20Z andrew $
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

#include "rs_actionblocksshowall.h"

#include "rs_graphic.h"



RS_ActionBlocksShowAll::RS_ActionBlocksShowAll(
        RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        bool hide)
        :RS_ActionInterface(container, graphicView) {

    this->hide = hide;
}



RS_GuiAction* RS_ActionBlocksShowAll::createGuiAction() {
    static RS_GuiAction* action = NULL;
    
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionBlocksShowAll::getName(), 
            
                "eyevisible", 
            tr("&Show all"));
        action->setStatusTip(tr("Shows all blocks"));
        action->setCommand("bs");
        action->setKeycode("bs");
        action->setFactory(RS_ActionBlocksShowAll::factory);
    }

    return action;
}


void RS_ActionBlocksShowAll::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



RS_ActionInterface* RS_ActionBlocksShowAll::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionBlocksShowAll(*container, *graphicView, false);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionBlocksShowAll::trigger() {
    RS_DEBUG->print("RS_ActionBlocksShowAll::trigger");
    if (graphic!=NULL) {
        graphic->freezeAllBlocks(hide);
    }
    graphicView->redraw();
    finish();
}

