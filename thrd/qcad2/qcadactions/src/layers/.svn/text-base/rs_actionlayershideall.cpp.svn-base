/****************************************************************************
** $Id: rs_actionlayersfreezeall.cpp 7068 2007-11-13 09:37:53Z andrew $
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

#include "rs_actionlayershideall.h"


RS_ActionLayersHideAll::RS_ActionLayersHideAll(
        RS_EntityContainer& container,
        RS_GraphicView& graphicView) 
    : RS_ActionLayersShowAll(container, graphicView, true) {
}



RS_GuiAction* RS_ActionLayersHideAll::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionLayersHideAll::getName(), 
            "eyehidden", 
            tr("&Hide All"));
        action->setStatusTip(tr("Hides (freezes) all layers"));
        action->setCommand("yh");
        action->setKeycode("yh,fr*");
        action->setFactory(RS_ActionLayersHideAll::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionLayersHideAll::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionLayersHideAll(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


