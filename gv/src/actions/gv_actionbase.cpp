/*****************************************************************
 * $Id: gv_actionbase.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Dec 23, 2008 6:42:21 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionbase.h"

#include "gv_actionsetsnapmodeauto.h"
#include "gv_actionsetsnaprestrictionnothing.h"
#include "gv_dialogfactory.h"
#include "gv_drawingwidget.h"
#include "gv_resizehandle.h"
#include "gv_symbol.h"

/**
 * Constructor
 */
GV_ActionBase::GV_ActionBase(GV_Drawing& dwg, GV_GraphicsView& gv)
    : GV_ActionComponent(dwg, gv) {

//    GV_ActionSetSnapModeAuto::factory();
//    GV_ActionSetSnapRestrictionNothing::factory();

}

/**
 * Destructor
 */
GV_ActionBase::~GV_ActionBase() {
    // nothing (yet)
    RB_DEBUG->print("GV_ActionBase::~GV_ActionBase OK");
}

/**
 * Called only once in the beginning of an action to initialize
 * stuff.
 */
void GV_ActionBase::init() {
    /*
    TODO-QCad
    RB_String cmd = RS_GuiAction::getCommandForAction(rtti());
    if (!cmd.isEmpty()) {
        RS_DIALOGFACTORY->commandMessage(cmd);
    }
    */
}

/**
 * Called to initialize an action to a new status.
 * @param status The status on which to initialize this action.
 * default is 0 to begin the action.
 */
void GV_ActionBase::init(int status) {
    RB_DEBUG->print("GV_ActionBase::init");
//    initSnapper(); TODO etc

    setStatus(status);
    if (status>=0) {
        updateMouseButtonHints();
        updateMouseCursor();
        updateToolBar();
    }
    else {
        //graphicView->redraw();
        updateToolBar();
    }

//    clearPreview();

}

/**
 * Called when the user starts dragging something into the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void GV_ActionBase::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    getDrawing()->sceneDragEnterEvent(e);
}

/**
 * Called when the user is dragging something out of the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation passes the does nothing.
 */
void GV_ActionBase::dragLeaveEvent(QGraphicsSceneDragDropEvent* e) {
    getDrawing()->sceneDragLeaveEvent(e);
}

/**
 * Called when the user is dragging something around in the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation passes the does nothing.
 */
void GV_ActionBase::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    getDrawing()->sceneDragMoveEvent(e);
}

/**
 * Called when the user is dropping something into the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void GV_ActionBase::dropEvent(QGraphicsSceneDragDropEvent* e) {
    getDrawing()->sceneDropEvent(e);
}

/**
 * Called when a key is pressed and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation transfers event to the scene.
 */
void GV_ActionBase::keyPressEvent(QKeyEvent* e) {
    getDrawing()->sceneKeyPressEvent(e);
}

/**
 * Called when a key is released and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void GV_ActionBase::keyReleaseEvent(QKeyEvent* e) {
    getDrawing()->sceneKeyReleaseEvent(e);

}

/**
 * Called when the mouse moves and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation executes QGraphicsScene::mouseMoveEvent
 */
void GV_ActionBase::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    getDrawing()->sceneMouseMoveEvent(e);
}

/**
 * Called when mouse button is pressed and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation executes QGraphicsScene::mousePressEvent and
 * selects an item if applicable.
 */
void GV_ActionBase::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    // To set selected item in scene
    getDrawing()->sceneMousePressEvent(e);
    // To set selected item in model
    setSelectedItem(e->scenePos());
}


/**
 * Mouse release event accepts the event and is transfered to the scene,
 * otherwise the mouseGrabber keep the first selected item
 */
void GV_ActionBase::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    getDrawing()->sceneMouseReleaseEvent(e);
}

/**
 * Coordinate event. Triggered usually from a command line.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void GV_ActionBase::coordinateEvent(GV_CoordinateEvent*) {}

/**
 * Called when a command from the command line is launched.
 * and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void GV_ActionBase::commandEvent(GV_CommandEvent*) {}


/**
 * @return Only those available commands that start with 'start'.
 * This is used to complete commands with tab.
 */
RB_StringList GV_ActionBase::getAvailableCommands(const RB_String& start) {
    RB_StringList ret;
    RB_StringList all = getAvailableCommands();

    for (int i=0; i<all.count(); ++i) {
        if (start.isEmpty() || all.at(i).startsWith(start)) {
            ret.append(all.at(i));
        }
    }

    return ret;
}

/**
 * Can be implemented to return the currently available commands
 * that can be triggered from the command line. The default implemetation
 * returns an empty list.
 */
RB_StringList GV_ActionBase::getAvailableCommands() {
    //RS_StringList cmd = RS_COMMANDS->getCommands();
    //return cmd;
    return RB_StringList();
}

/**
 * Triggers this action. This should be called after all
 * data needed for this action was collected / set.
 * The default implementation does nothing.
 */
void GV_ActionBase::trigger() {
    // nothing
}

/**
 * Should be overwritten to update the mouse button hints
 * wherever they might needed.
 */
void GV_ActionBase::updateMouseButtonHints() {}

/**
 * Should be overwritten to set the mouse cursor for this action.
 */
void GV_ActionBase::updateMouseCursor() {}

/**
 * Should be overwritten to set the toolbar for this action.
 */
void GV_ActionBase::updateToolBar() {}

/**
 * Called by the event handler to give this action a chance to
 * communicate with its predecessor.
 */
void GV_ActionBase::setPredecessor(GV_ActionBase* pre) {
    mPredecessor = pre;
}

/**
 * Suspends this action while another action takes place.
 */
void GV_ActionBase::suspend() {
    getGraphicsView()->setMouseCursor(Qt::ArrowCursor);
    GV_Snapper::suspend();
}

/**
 * Resumes an action after it was suspended.
 */
void GV_ActionBase::resume() {
    updateMouseCursor();
    updateMouseButtonHints();
    updateToolBar();
    GV_Snapper::resume();
}

/**
 * Snap to a coordinate in the drawing using the current snap mode.
 * @param e A mouse event.
 * @return The coordinates of the point or an invalid vector.
 */
RVector GV_ActionBase::snapPoint(QGraphicsSceneMouseEvent* e) {
    snapSpot = RVector();

    if (e==NULL) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
                        "GV_ActionBase::snapPoint(mouse): event is NULL");
        return snapSpot;
    }

//    if (getResizeHandle() && getResizeHandle()->parentItem()) {
//        GV_Symbol* symbol = dynamic_cast<GV_Symbol*>(getResizeHandle()->parentItem());
//        if (symbol) {
//            QPointF mousePoint = symbol->mapFromScene(e->scenePos().x(),
//                                                      e->scenePos().y());
//            QPointF dist = symbol->distanceToCorner(getResizeHandle(), mousePoint);
//            return snapPoint((int)(e->scenePos().x() - dist.x()),
//                             (int)(e->scenePos().y() - dist.y()));
//        }
//    }

    return snapPoint(e->scenePos().x(), e->scenePos().y());
}


/**
 * Snap to a coordinate in the drawing using the current snap mode.
 * @param e A drop event.
 * @return The coordinates of the point or an invalid vector.
 */
RVector GV_ActionBase::snapPoint(QGraphicsSceneDragDropEvent* e) {
    snapSpot = RVector();

    if (e==NULL) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
                        "GV_ActionBase::snapPoint(dragDrop): event is NULL");
        return snapSpot;
    }

    return snapPoint(e->scenePos().x(), e->scenePos().y());
}


/**
 * Snap to a point and add snap marker to preview.
 */
RVector GV_ActionBase::snapPoint(double x, double y) {

    RVector ret = GV_Snapper::snapPoint(x, y);
    //preview->setSnapMarker((RS_Snapper&)(*this),
    //    graphicView->getFactor().x);

    // ... refer to QCad2


    return ret;
}

