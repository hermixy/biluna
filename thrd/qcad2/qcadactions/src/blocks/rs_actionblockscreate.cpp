/****************************************************************************
** $Id: rs_actionblockscreate.cpp 9172 2008-02-23 15:25:20Z andrew $
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

#include "rs_actionblockscreate.h"

#include "rs_creation.h"
#include "rs_insert.h"
#include "rs_modification.h"
#include "rs_actionselect.h"

/**
 * Constructor.
 */
RS_ActionBlocksCreate::RS_ActionBlocksCreate(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface(container, graphicView) {

    referencePoint = RS_Vector(false);
}



RS_ActionBlocksCreate::~RS_ActionBlocksCreate() {}



RS_GuiAction* RS_ActionBlocksCreate::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionBlocksCreate::getName(),
            "blockcreate", 
            tr("&Create Block"));
        action->setStatusTip(tr("Creates a new block from existing entities"));
        action->setCommand("bc");
        action->setKeycode("bc");
        action->setFactory(RS_ActionBlocksCreate::factory);
    }                                   

    return action;
}



void RS_ActionBlocksCreate::init(int status) {
    RS_ActionInterface::init(status);

}



RS_ActionInterface* RS_ActionBlocksCreate::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a;
        if (container->countSelected()==0) {
            a = new RS_ActionSelect(*container, *graphicView, 
                RS_ActionBlocksCreate::factory);
        }
        else {
            a = new RS_ActionBlocksCreate(*container, *graphicView);
        }
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionBlocksCreate::trigger() {
    //deleteSnapper();
    //deletePreview();
    //clearPreview();

    //RS_Modification m(*container, graphicView);
    //m.paste(data.insertionPoint);
    //std::cout << *RS_Clipboard::instance();

    if (graphic!=NULL) {
        RS_BlockList* blockList = graphic->getBlockList();
        if (blockList!=NULL) {
            RS_BlockData d =
                RS_DIALOGFACTORY->requestNewBlockDialog(blockList);

            if (!d.name.isEmpty()) {
                RS_Creation creation(container, graphicView);
                creation.createBlock(d, referencePoint, true);

                RS_InsertData id(
                    d.name,
                    referencePoint,
                    RS_Vector(1.0,1.0),
                    0.0,
                    1, 1, RS_Vector(0.0,0.0)
                );
                creation.createInsert(id);
            }
        }
    }

    graphicView->redraw();
    finish();
    //graphicView->killSelectActions();
    RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
}


void RS_ActionBlocksCreate::mouseMoveEvent(RS_MouseEvent* e) {
    clearPreview();
    snapPoint(e);

    switch (getStatus()) {
    case SetReferencePoint:
        break;

    default:
        break;
    }

    drawPreview();
}



void RS_ActionBlocksCreate::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        //deleteSnapper();
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionBlocksCreate::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    switch (getStatus()) {
    case SetReferencePoint:
        referencePoint = e->getCoordinate();
        trigger();
        break;

    default:
        break;

    }
}



void RS_ActionBlocksCreate::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetReferencePoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionBlocksCreate::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionBlocksCreate::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}

