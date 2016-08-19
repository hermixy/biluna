/****************************************************************************
** $Id: rs_actiontoolregeneratedimensions.cpp 8386 2008-01-23 23:53:22Z andrew $
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

#include "rs_actiontoolregeneratedimensions.h"

#include "rs_information.h"
#include "rs_dimension.h"
#include "rs_leader.h"



RS_ActionToolRegenerateDimensions::RS_ActionToolRegenerateDimensions(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface(container, graphicView) {}


RS_GuiAction* RS_ActionToolRegenerateDimensions::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionToolRegenerateDimensions::getName(),  tr("&Regenerate Dimension Entities"));
        action->setStatusTip(tr("Regenerates all Dimension Entities"));
        action->setCommand("dimregen");
        action->setFactory(RS_ActionToolRegenerateDimensions::factory);
    }                                   

    return action;
}

void RS_ActionToolRegenerateDimensions::init(int status) {
    RS_ActionInterface::init(status);

    trigger();
}


RS_ActionInterface* RS_ActionToolRegenerateDimensions::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionToolRegenerateDimensions(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionToolRegenerateDimensions::trigger() {

    RS_DEBUG->print("RS_ActionToolRegenerateDimensions::trigger()");

    int num = container->updateDimensions(true, true);
    /*
    for (RS_Entity* e = container->firstEntity(RS2::ResolveNone);
            e != NULL;
            e = container->nextEntity(RS2::ResolveNone)) {

        if (e->isVisible()) {
            if (RS_Information::isDimension(e->rtti())) {
                num++;
                if (((RS_Dimension*)e)->getLabel()==";;") {
                    ((RS_Dimension*)e)->setLabel("");
                }
                ((RS_Dimension*)e)->update(true);
            }
            else if (e->rtti()==RS2::EntityDimLeader) {
                num++;
                ((RS_Leader*)e)->update();
            }
        }
    }
    */

    if (num>0) {
        graphicView->redraw();
        RS_DIALOGFACTORY->commandMessage(
            tr("Regenerated %1 dimension entities").arg(num));
    } else {
        RS_DIALOGFACTORY->commandMessage(tr("No dimension entities found"));
    }

    finish();
}


// EOF
