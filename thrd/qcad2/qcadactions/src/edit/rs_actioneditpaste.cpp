/****************************************************************************
** $Id: rs_actioneditpaste.cpp 9299 2008-03-03 14:12:57Z andrew $
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

#include "rs_actioneditpaste.h"


const char* RS_ActionEditPaste::propertyRotation = QT_TR_NOOP("Rotation");
const char* RS_ActionEditPaste::propertyFactor = QT_TR_NOOP("Factor");
const char* RS_ActionEditPaste::propertyFlipHorizontal = QT_TR_NOOP("Flip|Horizontal");
const char* RS_ActionEditPaste::propertyFlipVertical = QT_TR_NOOP("Flip|Vertical");
const char* RS_ActionEditPaste::propertyCurrentLayer = QT_TR_NOOP("To Current Layer");


/**
 * Constructor.
 *
 * @param undo true for undo and false for redo.
 */
RS_ActionEditPaste::RS_ActionEditPaste( RS_EntityContainer& container,
                                        RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {
                           
    /*
    RS_DEBUG->print(RS_Debug::D_ERROR, 
        "RS_ActionEditPaste::RS_ActionEditPaste: clipboard contents: ");
    RS_DEBUG->printEntity(
        RS_Debug::D_ERROR, RS_CLIPBOARD->getGraphic());
    */
}



RS_ActionEditPaste::~RS_ActionEditPaste() {}


RS_GuiAction* RS_ActionEditPaste::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionEditPaste::getName(), 
            "editpaste", 
                                  tr("&Paste"));
        action->setStatusTip(tr("Pastes the clipboard contents"));
        action->setShortcut("Ctrl+V");
        action->setCommand("paste,ps");
        action->setKeycode("ps");
        action->setFactory(RS_ActionEditPaste::factory);
    }
    return action;
}



RS_ActionInterface* RS_ActionEditPaste::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* action = new RS_ActionEditPaste(*container, *graphicView);
        graphicView->setCurrentAction(action);
        return action;
    }

    return NULL;
}



void RS_ActionEditPaste::init(int status) {
    RS_ActionBase::init(status);

    RS_SETTINGS->beginGroup("/EditPaste");
    RS_SETTINGS->writeEntry("/EditPasteRotation", 0.0);
    RS_SETTINGS->writeEntry("/EditPasteFactor", 1.0);
    RS_SETTINGS->writeEntry("/EditPasteFlipHorizontal", 0);
    RS_SETTINGS->writeEntry("/EditPasteFlipVertical", 0);
#ifdef RS_ARCH
    RS_SETTINGS->writeEntry("/EditPasteUseCurrentLayer", 1);
#else
    RS_SETTINGS->writeEntry("/EditPasteUseCurrentLayer", 0);
#endif
    RS_SETTINGS->endGroup();
            
    if (status>=0) {
        setRotation(0.0);
        setFactor(1.0);
        setFlipHorizontal(false);
        setFlipVertical(false);
#ifdef RS_ARCH
        setUseCurrentLayer(true);
#else
        setUseCurrentLayer(false);
#endif

#ifdef RS_ARCH
        RS_DIALOGFACTORY->updatePropertyEditor(this, false);
        RS_DIALOGFACTORY->updatePropertyEditorGui();
#endif
    }
}
    
    
void RS_ActionEditPaste::resume() {
    RS_ActionBase::resume();

    if (getStatus()>=0) {
#ifdef RS_ARCH
        RS_DIALOGFACTORY->updatePropertyEditor(this, false);
        RS_DIALOGFACTORY->updatePropertyEditorGui();
#endif
    }
}



void RS_ActionEditPaste::trigger() {
    clearPreview();

    data.angle = RS_Math::deg2rad(getRotation());
    data.factor = getFactor();
    data.flipHorizontal = getFlipHorizontal();
    data.flipVertical = getFlipVertical();
    data.useCurrentLayer = getUseCurrentLayer();

    RS_Modification m(*container, graphicView);
    m.paste(data);
        
    graphicView->redraw();
    finish();
}


void RS_ActionEditPaste::mouseMoveEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case SetTargetPoint: {
        clearPreview();
        data.insertionPoint = snapPoint(e);
        
        if (data.insertionPoint.valid) {
            RS_EntityContainer* sub = new RS_EntityContainer(preview);
            preview->addAllFrom(*RS_CLIPBOARD->getGraphic(), sub);
            sub->scale(RS_Vector(0,0), RS_Vector(getFactor(), getFactor()));
            sub->rotate(RS_Vector(0,0), RS_Math::deg2rad(getRotation()));
            if (getFlipHorizontal()) {
                sub->mirror(RS_Vector(0,0), RS_Vector(0,1));
            }
            if (getFlipVertical()) {
                sub->mirror(RS_Vector(0,0), RS_Vector(1,0));
            }
            sub->move(data.insertionPoint);
            if (getUseCurrentLayer()) {
                sub->setLayerToActive();
            }
                
            RS2::Unit sourceUnit = RS_CLIPBOARD->getGraphic()->getUnit();
            RS2::Unit targetUnit = graphic->getUnit();

            if (graphic!=NULL && sourceUnit!=targetUnit) {
                double f = RS_Units::convert(1.0, sourceUnit, targetUnit);
                sub->scale(data.insertionPoint, RS_Vector(f,f));
            }

            /*
            RS_DEBUG->print(RS_Debug::D_ERROR, 
                "RS_ActionEditPaste::mouseMoveEvent: sub container:");
            RS_DEBUG->printEntity(RS_Debug::D_ERROR, sub);
            */

            preview->addEntity(sub);
        }

        drawPreview();
        } break;

    default:
        break;
    }
}



void RS_ActionEditPaste::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionEditPaste::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    data.insertionPoint = e->getCoordinate();
    trigger();
}



void RS_ActionEditPaste::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetTargetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Set reference point"),
                                            tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionEditPaste::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionEditPaste::updateToolBar() {
    if (!isFinished()) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
    } else {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    }
}


/**
 * Reimplementation necessary to force order.
 */
RS_StringList RS_ActionEditPaste::getPropertyNames(bool /*includeGeometry*/) {
    RS_StringList ret;
    ret 
    << RS_ACTION_EDIT_PASTE_CURRENT_LAYER
    << RS_ACTION_EDIT_PASTE_ROTATION
    << RS_ACTION_EDIT_PASTE_FACTOR
    << RS_ACTION_EDIT_PASTE_FLIP_HORIZONTAL
    << RS_ACTION_EDIT_PASTE_FLIP_VERTICAL;
    return ret;
}
