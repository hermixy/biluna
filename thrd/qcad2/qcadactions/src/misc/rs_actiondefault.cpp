/****************************************************************************
** $Id: rs_actiondefault.cpp 9429 2008-03-19 10:15:18Z andrew $
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

#include "rs_actiondefault.h"

#include "rs.h"
#include "rs_modification.h"
#include "rs_settings.h"
#include "rs_snapper.h"
#include "rs_selection.h"
//#include "rs_actionsetsnaprestrictionorthogonal.h"
//#include "rs_actionsetsnaprestrictionnothing.h"



/**
 * Constructor.
 */
RS_ActionDefault::RS_ActionDefault(RS_EntityContainer& container,
                                   RS_GraphicView& graphicView)
        : RS_ActionInterface(
                            container, graphicView) {

    RS_DEBUG->print("RS_ActionDefault::RS_ActionDefault");
    highlightedEntity = NULL;
    persistentSelections = false;
    highlightOnMouseOver = true;
    RS_DEBUG->print("RS_ActionDefault::RS_ActionDefault: OK");
}



RS_ActionDefault::~RS_ActionDefault() {
}



RS_GuiAction* RS_ActionDefault::createGuiAction() {
    return NULL;
}


void RS_ActionDefault::init(int status) {
    RS_DEBUG->print("RS_ActionDefault::init");
    
    RS_ActionInterface::init(status);
    v1 = v2 = RS_Vector(false);
    snapMode = RS2::ActionSnapFree;
    //snapRes = RS2::ActionRestrictNothing;
    //restrBak = RS2::ActionRestrictNothing;
    //ortho = false;
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    
    RS_DEBUG->print("RS_ActionDefault::init: OK");
}



void RS_ActionDefault::suspend() {
    RS_ActionInterface::suspend();
    highlightedEntity = NULL;
    clearPreview();
    drawPreview();
}



void RS_ActionDefault::resume() {
    RS_ActionInterface::resume();
    highlightedEntity = NULL;
    if (graphicView!=NULL) {
        graphicView->emulateMouseMoveEvent();
    }
        
    RS_SETTINGS->beginGroup("/Behavior");
    persistentSelections = RS_SETTINGS->readNumEntry("/PersistentSelections", 0);
    highlightOnMouseOver = RS_SETTINGS->readNumEntry("/HighlightOnMouseOver", 1);
    RS_SETTINGS->endGroup();

}



RS_ActionInterface* RS_ActionDefault::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDefault(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDefault::trigger() {
    RS_ActionInterface::trigger();
}

/*
void RS_ActionDefault::keyPressEvent(RS_KeyEvent* e) {
    if (e->key()==Qt::Key_Shift) {
        //RS_DEBUG->print("RS_ActionDefault::keyPressEvent: ortho mode");
        //restrBak = graphicView->getSnapRestriction();
        //graphicView->setSnapRestriction(RS2::ActionRestrictOrthogonal);
        //setSnapRestriction(RS2::ActionRestrictOrthogonal);
    }
    else {
        e->ignore();
    }
}
*/

void RS_ActionDefault::keyReleaseEvent(RS_KeyEvent* e) {
    if (e->key()==Qt::Key_Shift) {
        //graphicView->setSnapRestriction(restrBak);
    }
    else if (e->key()==Qt::Key_Escape) {
        RS_SETTINGS->beginGroup("/Behavior");
        bool persistentSelections = RS_SETTINGS->readNumEntry("/PersistentSelections", 0);
        RS_SETTINGS->endGroup();

        if (!persistentSelections) {
            RS_Selection s(*container, graphicView);
            s.deselectAll();
            highlightedEntity = NULL;
        }
        else {
            e->ignore();
        }
    }
    else {
        e->ignore();
    }
}



void RS_ActionDefault::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent");

    /*
    if (e->modifiers()==Qt::ShiftModifier) {
        RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent: shift -> ortho");
        if (ortho==false && graphicView->getSnapRestriction()==RS2::ActionRestrictNothing) {
            //restrBak = graphicView->getSnapRestriction();
            //graphicView->setSnapRestriction(RS2::ActionRestrictOrthogonal);
            RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent: ortho");
            ortho = true;
            RS_ActionSetSnapRestrictionOrthogonal::factory();
        }
    }
    else {
        if (ortho==true) {
            //graphicView->setSnapRestriction(RS2::ActionRestrictNothing);
            RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent: ortho off");
            ortho = false;
            RS_ActionSetSnapRestrictionNothing::factory();
        }
    }
    */

    RS_Vector mouse = graphicView->toGraph(RS_Vector(e->x(), e->y()));
    RS_Vector relMouse = mouse - graphicView->getRelativeZero();
    
    RS_DIALOGFACTORY->updateCoordinateWidget(mouse, relMouse);

    switch (getStatus()) {
    case Dragging:
        v2 = mouse;

        if (graphicView->toGuiDX(v1.distanceTo(v2))>10) {
            // look for reference points to drag:
            double dist;
            RS_Vector ref = container->getNearestSelectedRef(v1, &dist);
            if (ref.valid==true && graphicView->toGuiDX(dist)<8) {
                RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent: "
                    "moving reference point");
                setStatus(MovingRef);
                RS_DIALOGFACTORY->enablePropertyEditor(false);
                v1 = ref;
                graphicView->moveRelativeZero(v1);
                graphicView->restore();
                showOptions();
            } 
            else {
                // look for an entity to drag:
                RS_Entity* en = catchEntity(v1);
                if (en!=NULL && en->isSelected()) {
                    RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent: "
                        "moving entity");
                    setStatus(Moving);
                    RS_DIALOGFACTORY->enablePropertyEditor(false);
                    v1 = en->getNearestRef(v1, NULL, RS2::RefMoving);
                    graphicView->moveRelativeZero(v1);
                    graphicView->restore();
                    showOptions();
                }

                // no entity found. start area selection:
                else {
                    RS_DIALOGFACTORY->enablePropertyEditor(true);
                    setStatus(SetCorner2);
                    hideOptions();
                }
            }
        }
        break;
        
    case MovingRef:
        clearPreview();
        v2 = snapPoint(e);
        highlightedEntity = NULL;
        if (v2.valid) {
            preview->addSelectionFrom(*container);
            preview->moveRef(v1, v2-v1);
            drawPreview();
        }
        break;

    case Moving:
        clearPreview();
        v2 = snapPoint(e);
        highlightedEntity = NULL;
        if (v2.valid) {
            RS_EntityContainer* subContainer = 
                new RS_EntityContainer(preview);
            preview->addSelectionFrom(*container, subContainer);
            subContainer->move(v2-v1);
            preview->addEntity(subContainer);
            //RS_DEBUG->printEntity(RS_Debug::D_ERROR, preview);
            drawPreview();
        }
        break;

    case SetCorner2:
        highlightedEntity = NULL;
        clearPreview();
        if (v1.valid) {
            v2 = mouse;

            preview->addRectangle(v1, v2, NULL, true);
            drawPreview();
        }

    default: 
        break;
    }
            
    // highlight closest entity (on mouse rollover highlighting):
    if (highlightOnMouseOver) {
        if (getStatus()==Neutral || getStatus()==Dragging) {
            RS_Entity* en = catchEntity(mouse);
            if (en!=NULL && !en->isSelected()) {
                if (en!=highlightedEntity) {
                    highlightedEntity = en;
                    clearPreview();
                    RS_Entity* cl = en->clone();
                    // 20080212: huge performance increase without update here:
                    //cl->update();
                    cl->setUpdateEnabled(false);

                    RS_Pen pen = cl->getPen();
                    RS_Color col = graphicView->getSelectedColor();
                    col.setAlpha(225);
                    pen.setColor(col);
                    cl->setOverridePen(pen);
                    
                    preview->addPreviewEntity(cl);
                    drawPreview();
                }
            }
            else {
                if (preview->count()!=0) {
                    highlightedEntity = NULL;
                    clearPreview();
                    drawPreview();
                }
            }
        }
    }
    
    RS_DEBUG->print("RS_ActionDefault::mouseMoveEvent: OK");
}



void RS_ActionDefault::mousePressEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDefault::mousePressEvent");

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case Neutral:
            v1 = graphicView->toGraph(e->x(), e->y());
            RS_DIALOGFACTORY->enablePropertyEditor(true);
            setStatus(Dragging);
            break;

        case Moving:
        case MovingRef: {
                RS_CoordinateEvent ce(snapPoint(e));
                if (ce.isValid()) {
                    coordinateEvent(&ce);
                }
                graphicView->emulateMouseMoveEvent();
            }
            break;

        default:
            break;
        }
    }
    RS_DEBUG->print("RS_ActionDefault::mousePressEvent: OK");
}



void RS_ActionDefault::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDefault::mouseReleaseEvent");
    
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case Dragging: {
            RS_DEBUG->print("RS_ActionDefault::mouseReleaseEvent: "
                "trying to select entity");

            // select single entity:
            RS_Entity* en = catchEntity(e);
                
            if (en!=NULL) {
                clearPreview();
    
                RS_Selection s(*container, graphicView);
                if (!persistentSelections) {
                    if (!en->isSelected()) {
                        if ((int)(e->modifiers()&Qt::ShiftModifier)==0 &&
                            (int)(e->modifiers()&Qt::ControlModifier)==0) {
                            s.deselectAll(false);
                        }
                        s.selectSingle(en);
                    }
                    else {
                        if ((int)(e->modifiers()&Qt::ShiftModifier)==Qt::ShiftModifier ||
                            (int)(e->modifiers()&Qt::ControlModifier)==Qt::ControlModifier) {
                            s.selectSingle(en);
                        }
                    }
                }
                else {
                    s.selectSingle(en);
                    highlightedEntity = NULL;
                }

                RS_DIALOGFACTORY->updateSelectionWidget(
                    container->countSelected());
                RS_DIALOGFACTORY->enablePropertyEditor(true);

                setStatus(Neutral);
            } else {
                // click beside entities: 
                if (!persistentSelections) {
                    RS_Selection s(*container, graphicView);
                    s.deselectAll();
                    highlightedEntity = NULL;
                    RS_DIALOGFACTORY->updateSelectionWidget(
                        container->countSelected());
                    setStatus(Neutral);
                }
                else {
                    RS_DIALOGFACTORY->enablePropertyEditor(true);
                    setStatus(SetCorner2);
                }
            }
        }
        break;

        case SetCorner2: {
            v2 = graphicView->toGraph(e->x(), e->y());

            clearPreview();
        
            RS_Selection s(*container, graphicView);
            if (!persistentSelections &&
                (int)(e->modifiers()&Qt::ShiftModifier)==0 &&
                (int)(e->modifiers()&Qt::ControlModifier)==0) {
                s.deselectAll(false);
            }

            s.selectWindow(v1, v2, true);

            RS_DIALOGFACTORY->updateSelectionWidget(
                container->countSelected());

            RS_DIALOGFACTORY->enablePropertyEditor(true);
            setStatus(Neutral);
        }
        break;
            
        default:
            break;
        }

    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        switch (getStatus()) {
        case SetCorner2:
        case Moving:
        case MovingRef:
            //deletePreview();
            clearPreview();
            hideOptions();
            RS_DIALOGFACTORY->enablePropertyEditor(true);
            setStatus(Neutral);
            graphicView->restore();
            RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
            e->accept();
            break;

        default:
            RS_DIALOGFACTORY->requestPreviousMenu();
            e->accept();
            break;
        }
    }

    RS_DEBUG->print("RS_ActionDefault::mouseReleaseEvent: OK");
}


void RS_ActionDefault::coordinateEvent(RS_CoordinateEvent* e) {
    RS_DEBUG->print("RS_ActionDefault::coordinateEvent");
    
    if (e==NULL) {
        RS_DEBUG->print("RS_ActionDefault::coordinateEvent: event was NULL");
        return;
    }

    RS_Vector mouse = e->getCoordinate();
    RS_DEBUG->print("RS_ActionDefault::coordinateEvent: mouse: %f/%f. valid: %d", 
        mouse.x, mouse.y, mouse.valid);

    switch (getStatus()) {
    case Moving: {
            v2 = mouse;
            if (v2.valid) {
                clearPreview();
                RS_Modification m(*container, graphicView);
                RS_MoveData data;
                data.number = 0;
                data.useCurrentLayer = false;
                data.useCurrentAttributes = false;
                data.offset = v2-v1;
                m.move(data);
                setStatus(Neutral);
                hideOptions();
                RS_DIALOGFACTORY->enablePropertyEditor(true);
                RS_DIALOGFACTORY->updateSelectionWidget(
                        container->countSelected());
                RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
            }
        }
        break;
        
    case MovingRef: {
            v2 = mouse;
            if (v2.valid) {
                clearPreview();
                RS_Modification m(*container, graphicView);
                RS_MoveRefData data;
                data.ref = v1;
                data.offset = v2-v1;
                m.moveRef(data);
                //container->moveSelectedRef(v1, v2-v2);
                setStatus(Neutral);
                hideOptions();
                RS_DIALOGFACTORY->enablePropertyEditor(true);
                RS_DIALOGFACTORY->updateSelectionWidget(
                        container->countSelected());
                RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
            }
        }
        break;

    default:
        break;

    }
    RS_DEBUG->print("RS_ActionDefault::coordinateEvent: OK");
}



void RS_ActionDefault::commandEvent(RS_CommandEvent*) {}



/**
 * @returns all available top-level command that can be used to trigger
 *  new actions.
 */
RS_StringList RS_ActionDefault::getAvailableCommands() {
    return RS_GuiAction::getAvailableCommands(true);
}


void RS_ActionDefault::updateMouseButtonHints() {
    switch (getStatus()) {
    case Neutral:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    case SetCorner2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Choose second edge"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDefault::updateMouseCursor() {
    switch (getStatus()) {
    case Neutral:
        graphicView->setMouseCursor(RS2::ArrowCursor);
        break;
    case Moving:
    case MovingRef:
        graphicView->setMouseCursor(RS2::SelectCursor);
        break;
    default:
        break;
    }
}



void RS_ActionDefault::updateToolBar() {
    //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
    switch (getStatus()) {
    case Neutral:
        // would switch back to main in edit / measure / .. modes
        //RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        break;
    case Moving:
    case MovingRef:
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
        break;
    default:
        break;
    }
}


/**
 * Don't show the snap options in the default action.
 */
void RS_ActionDefault::showOptions() {
    switch (getStatus()) {
    case Moving:
    case MovingRef:
        RS_ActionInterface::showOptions();
        break;
    default:
        break;
    }
}
