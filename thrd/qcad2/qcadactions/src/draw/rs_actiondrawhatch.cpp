/****************************************************************************
** $Id: rs_actiondrawhatch.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawhatch.h"

#include "rs_graphicview.h"
#include "rs_information.h"
#include "rs_settings.h"
#include "rs_snapper.h"



RS_ActionDrawHatch::RS_ActionDrawHatch(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
        :RS_ActionBase(
                           container, graphicView) {

    hatch = NULL;
}


RS_GuiAction* RS_ActionDrawHatch::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawHatch::getName(), 
            "menuhatch", 
            tr("&Hatch"));
        action->setStatusTip(tr("Draw hatches and solid fills"));
        action->setCommand("hatch,ha");
        action->setKeycode("ha");
        action->setFactory(RS_ActionDrawHatch::factory);
    }                                   
    return action;
}


void RS_ActionDrawHatch::init(int status) {
    RS_ActionBase::init(status);

    //clearPreview();

    RS_Hatch tmp(container, data);
    tmp.setLayerToActive();
    tmp.setPenToActive();
    if (RS_DIALOGFACTORY->requestHatchDialog(&tmp)) {
        data = tmp.getData();
        trigger();
        finish();
        graphicView->redraw();
    } else {
        finish();
    }
}



/**
 * @todo: automatically explode ellipses to hatch them.
 */
RS_ActionInterface* RS_ActionDrawHatch::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawHatch(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawHatch::trigger() {

    RS_DEBUG->print("RS_ActionDrawHatch::trigger()");

    RS_Entity* e;
                
    // deselect unhatchable entities:
    for (e=container->firstEntity(RS2::ResolveNone); e!=NULL;
            e=container->nextEntity(RS2::ResolveNone)) {
        if (e->isSelected() && 
            (e->rtti()==RS2::EntityHatch ||
             e->rtti()==RS2::EntityEllipse || 
             e->rtti()==RS2::EntityPoint ||
             e->rtti()==RS2::EntityText || 
             e->isOfType(RS2::EntityDimension))) {
            e->setSelected(false);
        }
    }
    for (e=container->firstEntity(RS2::ResolveAll); e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {
        if (e->isSelected() && 
            (e->rtti()==RS2::EntityHatch ||
             e->rtti()==RS2::EntityEllipse || 
             e->rtti()==RS2::EntityPoint ||
             e->rtti()==RS2::EntityText || 
             e->isOfType(RS2::EntityDimension))) {
            e->setSelected(false);
        }
    }

    // look for selected contours:
    bool haveContour = false;
    for (e=container->firstEntity(RS2::ResolveAll); e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {
        if (e->isSelected()) {
            haveContour = true;
        }
    }

    if (!haveContour) {
        RS_DIALOGFACTORY->commandMessage(tr("No contour selected"));
        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
        return;
    }

    hatch = new RS_Hatch(container, data);
    hatch->setLayerToActive();
    hatch->setPenToActive();
    RS_EntityContainer* loop = new RS_EntityContainer(hatch);
    loop->setPen(RS_Pen(RS2::FlagInvalid));

    // add selected contour:
    for (RS_Entity* e=container->firstEntity(RS2::ResolveAll); e!=NULL;
            e=container->nextEntity(RS2::ResolveAll)) {
            
        if (e->isSelected()) {
            e->setSelected(false);
            RS_Entity* cp;
            cp = e->clone();
            cp->setPen(RS_Pen(RS2::FlagInvalid));
            cp->reparent(loop);
            loop->addEntity(cp);
        }
    }

    container->setSelected(false);

    hatch->addEntity(loop);
    if (hatch->validate()) {
        container->addEntity(hatch);

        if (document) {
            document->startUndoCycle();
            document->addUndoable(hatch);
            document->endUndoCycle();
        }
        hatch->update();

        graphicView->createPainter();
        graphicView->drawEntity(hatch);
        graphicView->restore();
        
        RS_DIALOGFACTORY->commandMessage(tr("Hatch created successfully."));
    }
    else {
        delete hatch;
        hatch = NULL;
        RS_DIALOGFACTORY->commandMessage(tr("Invalid hatch area. Please check that "
        "the entities chosen form one or more closed contours."));
    }
    
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}



void RS_ActionDrawHatch::mouseMoveEvent(RS_MouseEvent*) {
    RS_DEBUG->print("RS_ActionDrawHatch::mouseMoveEvent begin");

    RS_DEBUG->print("RS_ActionDrawHatch::mouseMoveEvent end");
}



void RS_ActionDrawHatch::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_Vector mouse = snapPoint(e);

        switch (getStatus()) {
        case ShowDialog:
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionDrawHatch::updateMouseButtonHints() {
    RS_DIALOGFACTORY->updateMouseWidget("", "");
}



void RS_ActionDrawHatch::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawHatch::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
}

