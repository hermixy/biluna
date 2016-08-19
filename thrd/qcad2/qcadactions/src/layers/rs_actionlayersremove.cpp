/****************************************************************************
** $Id: rs_actionlayersremove.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionlayersremove.h"

#include "rs_graphic.h"



RS_ActionLayersRemove::RS_ActionLayersRemove(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface( container, graphicView) {}


RS_GuiAction* RS_ActionLayersRemove::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionLayersRemove::getName(), 
            "layerremove", 
            tr("&Remove Layer"));
        action->setStatusTip(tr("Removes the currently active layer"));
        action->setCommand("yr");
        action->setKeycode("yr");
        action->setFactory(RS_ActionLayersRemove::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionLayersRemove::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionLayersRemove(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionLayersRemove::trigger() {
    RS_DEBUG->print("RS_ActionLayersRemove::trigger");

    if (graphic!=NULL) {
        RS_Layer* layer =
            RS_DIALOGFACTORY->requestLayerRemovalDialog(graphic->getLayerList());

        // Now remove the layer from the layer list:
        graphic->removeLayer(layer);
    }
    finish();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionLayersRemove::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

