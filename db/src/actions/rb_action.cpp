/*****************************************************************
 * $Id: rb_action.cpp 1988 2013-09-05 13:39:53Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_action.h"

#include "db_commandevent.h"
#include "rb_debug.h"
#include "rb_dialog.h"
#include "rb_widget.h"

int RB_Action::mUntitledNumber = 0;

/**
 * Default constructor
 */
RB_Action::RB_Action() {
    RB_DEBUG->print("RB_Action::RB_Action()");

    mFinished = false;
    mPredecessor = NULL;
    mWidget = NULL;

    mStatus = 0;
}

/**
 * Destructor.
 */
RB_Action::~RB_Action() {
    // would be pure virtual now:
    // hideOptions();
}

/**
 * Called only once in the beginning of an action to initialize
 * stuff.
 */
void RB_Action::init() {
    /*
    TODO
    RB_String cmd = RB_GuiAction::getCommandForAction(rtti());
    if (!cmd.isEmpty()) {
        RB_DIALOGFACTORY->commandMessage(cmd);
    }
    */
}


/**
 * Called to initialize an action to a new status.
 *
 * @param status The status on which to initialize this action.
 * default is 0 to begin the action.
 */
void RB_Action::init(int status) {
    RB_DEBUG->print("RB_Action::init");
//    initSnapper();

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

    RB_DEBUG->print("RB_Action::init: OK");
}



/**
 * Called when the mouse moves and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation keeps track of the mouse position.
 */
void RB_Action::mouseMoveEvent(QMouseEvent*) {}

/**
 * Called when the left mouse button is pressed and this is the
 * current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RB_Action::mousePressEvent(QMouseEvent*) {}

/**
 * Called when the left mouse button is released and this is
 * the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RB_Action::mouseReleaseEvent(QMouseEvent*) {}

/**
 * Called when a key is pressed and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RB_Action::keyPressEvent(QKeyEvent* e) {
    e->ignore();
}

/**
 * Called when a key is released and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RB_Action::keyReleaseEvent(QKeyEvent* e) {
    e->ignore();
}


/**
 * Called when the user starts dragging something into the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation ignores the event.
 */
void RB_Action::dragEnterEvent(QDragEnterEvent* e) {
    e->ignore();
}


/**
 * Called when the user is dragging something around in the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation ignores the event.
 */
void RB_Action::dragMoveEvent(QDragMoveEvent* e) {
    e->ignore();
}

/**
 * Called when the user is dropping something into the drawing
 * canvas.
 * This function can be overwritten by the implementing action.
 * The default implementation ignores the event.
 */
void RB_Action::dropEvent(QDropEvent* e) {
    e->ignore();
}

/**
 * Coordinate event. Triggered usually from a command line.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
//void RB_Action::coordinateEvent(RB_CoordinateEvent*) {}

/**
 * Called when a command from the command line is launched.
 * and this is the current action.
 * This function can be overwritten by the implementing action.
 * The default implementation does nothing.
 */
void RB_Action::commandEvent(DB_CommandEvent*) {}


/**
 * @return Only those available commands that start with 'start'.
 * This is used to complete commands with tab.
 */
RB_StringList RB_Action::getAvailableCommands(const RB_String& start) {
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
RB_StringList RB_Action::getAvailableCommands() {
    return RB_StringList();
}

/**
 * Sets the current status (progress) of this action.
 * The default implementation sets the class variable 'status' to the
 * given value and finishes the action if 'status' is negative.
 *
 * @param status Status number. It's up to the action implementor
 *               what the action uses the status for. However, a
 *               negative status number finishes the action. Usually
 *               the status of an action increases for every step
 *               of progress and decreases when the user goes one
 *               step back (i.e. presses the right mouse button).
 */
void RB_Action::setStatus(int status) {
    this->mStatus = status;
    if (status<0) {
        finish();
        mStatus=0;
    }
    else {
        updateMouseButtonHints();
        updateToolBar();
        updateMouseCursor();
        if (status!=0) {
//            graphicView->emulateMouseMoveEvent();
        }
    }
}

/**
 * @return Current status of this action.
 */
int RB_Action::getStatus() {
    return mStatus;
}

/**
 * Triggers this action. This should be called after all
 * data needed for this action was collected / set.
 * The default implementation does nothing.
 */
void RB_Action::trigger() {}

/**
 * Should be overwritten to update the mouse button hints
 * wherever they might needed.
 */
void RB_Action::updateMouseButtonHints() {}

/**
 * Should be overwritten to set the mouse cursor for this action.
 */
void RB_Action::updateMouseCursor() {}

/**
 * Should be overwritten to set the toolbar for this action.
 */
void RB_Action::updateToolBar() {}

/**
 * @return true, if the action is mFinished and can be deleted.
 */
bool RB_Action::isFinished() {
    return mFinished;
}

/**
 * Forces a termination of the action without any cleanup.
 */
void RB_Action::setFinished() {
    mStatus = -1;
}

/**
 * Finishes this action.
 */
void RB_Action::finish() {

    mStatus = -1;
    mFinished = true;

    RB_DEBUG->print("RB_Action::finish: OK");
}

/**
 * Called by the event handler to give this action a chance to
 * communicate with its predecessor.
 */
void RB_Action::setPredecessor(RB_Action* pre) {
    mPredecessor = pre;
}

/**
 * Suspends this action while another action takes place.
 */
void RB_Action::suspend() {
//    graphicView->setMouseCursor(RS2::ArrowCursor);
//    RB_Snapper::suspend();
}



/**
 * Resumes an action after it was suspended.
 */
void RB_Action::resume() {
    updateMouseCursor();
    updateMouseButtonHints();
    updateToolBar();
//    RB_Snapper::resume();
}

/**
 * Set specific widget which should be subject of this action
 * @param wdgt subject widget
 */
void RB_Action::setWidget(RB_Widget* wdgt) {
    mWidget = wdgt;
}
