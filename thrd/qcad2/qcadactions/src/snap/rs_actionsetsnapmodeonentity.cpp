/****************************************************************************
** $Id: rs_actionsetsnapmode.cpp 7179 2007-11-16 15:09:23Z andrew $
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

#include "rs_actionsetsnapmodeonentity.h"



RS_GuiAction* RS_ActionSetSnapModeOnEntity::action = NULL;



/**
 * Constructor.
 */
RS_ActionSetSnapModeOnEntity::RS_ActionSetSnapModeOnEntity(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
    : RS_ActionSetSnapMode(container, graphicView, RS2::ActionSnapOnEntity) {
}



RS_GuiAction* RS_ActionSetSnapModeOnEntity::createGuiAction() {
    

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionSetSnapModeOnEntity::getName(),
            
                "snaponentity", tr("&On Entity"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setStatusTip(tr("Snap to nearest point on entity"));
        action->setCommand("near,nea,st,np");
        action->setKeycode("st,np");
        action->setFactory(RS_ActionSetSnapModeOnEntity::factory);

        actionGroup->addAction(action);
    }

    return action;
}



RS_ActionInterface* RS_ActionSetSnapModeOnEntity::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSetSnapModeOnEntity(
            *container, *graphicView);
        graphicView->setCurrentAction(a);

        if (action!=NULL) {
            action->setChecked(true);
        }
        
        return a;
    }

    return NULL;
}
