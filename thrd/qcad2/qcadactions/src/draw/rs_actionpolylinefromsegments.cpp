/****************************************************************************
** $Id: rs_actiondrawlineparallel.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionpolylinefromsegments.h"

#include "rs_creation.h"
#include "rs_snapper.h"
#include "rs_modification.h"



RS_ActionPolylineFromSegments::RS_ActionPolylineFromSegments(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase( container, graphicView) {

    entity = NULL;
}

RS_GuiAction* RS_ActionPolylineFromSegments::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineFromSegments::getName(), 
            "polylinefromsegments",
            tr("Create From Se&gments"));
        action->setStatusTip(tr("Create polylines from existing segments"));
        action->setCommand("og");
        action->setKeycode("og");
        action->setFactory(RS_ActionPolylineFromSegments::factory);
    }

    return action;
}


RS_ActionInterface* RS_ActionPolylineFromSegments::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineFromSegments(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionPolylineFromSegments::trigger() {
    RS_ActionBase::trigger();

    if (entity!=NULL && 
        (entity->rtti()==RS2::EntityLine || entity->rtti()==RS2::EntityArc)) {

        RS_Modification m(*container, graphicView);
        m.segmentsToPolyline(*(dynamic_cast<RS_AtomicEntity*>(entity)));

        entity = NULL;
    }
}



void RS_ActionPolylineFromSegments::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineFromSegments::mouseMoveEvent begin");

    entity = catchEntity(e, RS2::ResolveNone);

    switch (getStatus()) {
    case SetEntity: {
            deletePreview();
            clearPreview();

            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionPolylineFromSegments::mouseMoveEvent end");
}



void RS_ActionPolylineFromSegments::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    } else {
        entity = catchEntity(e, RS2::ResolveNone);
        trigger();
    }
}



void RS_ActionPolylineFromSegments::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetEntity:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify a segment"),
                tr("Cancel"));
            break;

        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



void RS_ActionPolylineFromSegments::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }
}



RS_StringList RS_ActionPolylineFromSegments::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    return cmd;
}



void RS_ActionPolylineFromSegments::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionPolylineFromSegments::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

