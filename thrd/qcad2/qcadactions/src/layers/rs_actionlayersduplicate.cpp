/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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

#include "rs_actionlayersduplicate.h"
#include "rs.h"
#include "rs_modification.h"


RS_ActionLayersDuplicate::RS_ActionLayersDuplicate(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView)
        :RS_ActionInterface(
                           container, graphicView) {

    RS_DEBUG->print("RS_ActionLayersDuplicate::RS_ActionLayersDuplicate");
    reset();
    RS_DEBUG->print("RS_ActionLayersDuplicate::RS_ActionLayersDuplicate: OK");
}



RS_ActionLayersDuplicate::~RS_ActionLayersDuplicate() {}


RS_GuiAction* RS_ActionLayersDuplicate::createGuiAction() {
    static RS_GuiAction* action = NULL;
    
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionLayersDuplicate::getName(), 
                                "layerduplicate",
                             tr("&Duplicate Floor"));
        action->setShortcut("Ctrl+D");
        action->setStatusTip(tr("Duplicates the current floor"));
        action->setFactory(RS_ActionLayersDuplicate::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionLayersDuplicate::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = 
            new RS_ActionLayersDuplicate(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionLayersDuplicate::reset() {
}



void RS_ActionLayersDuplicate::init(int status) {
    RS_ActionInterface::init(status);
    reset();
    trigger();
}


void RS_ActionLayersDuplicate::trigger() {
    RS_ActionInterface::trigger();
    
    
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    if (container==NULL) {
        return;
    }

    RS_Graphic* graphic = container->getGraphic();
    if (graphic==NULL) {
        return;
    }

    RS_LayerList* layerList = graphic->getLayerList();
    if (layerList==NULL) {
        return;
    }

    RS_Layer* original = layerList->getActive();
    if (original==NULL) {
        return;
    }
    
    RS_Modification m(*container, graphicView);
    m.duplicateLayer(original);

    finish();
}


