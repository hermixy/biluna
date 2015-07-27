/*****************************************************************
 * $Id: gv_eventhandler.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 7, 2008 8:14:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_eventhandler.cpp 10610
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_eventhandler.h"

//#include "gv_actionselectbase.h"
// #include "rs_regexp.h" is now same as QRegExp
#include "rb_debug.h"
#include "RMath.h"

/**
 * Constructor.
 */
GV_EventHandler::GV_EventHandler(GV_Drawing* dwg) {
    this->mDrawing = dwg;
    actionIndex=-1;
    for (int i=0; i<GV_MAXACTIONS; ++i) {
        currentActions[i] = NULL;
    }
    coordinateInputEnabled = true;
    defaultAction = NULL;
}

/**
 * Destructor.
 */
GV_EventHandler::~GV_EventHandler() {
    RB_DEBUG->print("GV_EventHandler::~GV_EventHandler");
	if (defaultAction!=NULL) {
		defaultAction->finish();
        delete defaultAction;
        defaultAction = NULL;
    }

    killAllActions();
    RB_DEBUG->print("GV_EventHandler::~GV_EventHandler: OK");
}

/**
 * Go back in current action.
 */
void GV_EventHandler::back() {
//    QMouseEvent e(QEvent::MouseButtonRelease, QPoint(0,0),
//                    Qt::RightButton, Qt::MouseButtons(Qt::RightButton),
//                    Qt::KeyboardModifiers(Qt::NoModifier));
    QGraphicsSceneMouseEvent e(QEvent::User);
    mouseReleaseEvent(&e);
}

/**
 * Go enter pressed event for current action.
 */
void GV_EventHandler::enter() {
    QKeyEvent e(QEvent::KeyPress, Qt::Key_Enter,
                  Qt::KeyboardModifiers(Qt::NoModifier));
    keyPressEvent(&e);
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->dragEnterEvent(e);
    } else {
        if (defaultAction!=NULL) {
            defaultAction->dragEnterEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->dragMoveEvent(e);
    } else {
        if (defaultAction!=NULL) {
            defaultAction->dragMoveEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::dragLeaveEvent(QGraphicsSceneDragDropEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->dragLeaveEvent(e);
    } else {
        if (defaultAction!=NULL) {
            defaultAction->dragLeaveEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::dropEvent(QGraphicsSceneDragDropEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->dropEvent(e);
    } else {
        if (defaultAction!=NULL) {
            defaultAction->dropEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::keyPressEvent(QKeyEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->keyPressEvent(e);
    } else {
        if (defaultAction!=NULL) {
            defaultAction->keyPressEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::keyReleaseEvent(QKeyEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->keyReleaseEvent(e);
    } else {
        if (defaultAction!=NULL) {
            defaultAction->keyReleaseEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::mouseEnterEvent() {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->resume();
    } else {
        if (defaultAction!=NULL) {
            defaultAction->resume();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::mouseLeaveEvent() {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->suspend();
    } else {
        if (defaultAction!=NULL) {
            defaultAction->suspend();
        }
    }
}

/**
 * Called by GV_Scene
 */
void GV_EventHandler::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
        currentActions[actionIndex]->mouseMoveEvent(e);
        e->accept();
    } else {
        if (defaultAction!=NULL) {
            defaultAction->mouseMoveEvent(e);
            e->accept();
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene 
 */
void GV_EventHandler::mousePressEvent(QGraphicsSceneMouseEvent* e) {
	if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {
		currentActions[actionIndex]->mousePressEvent(e);
        e->accept();
    } else {
        if (defaultAction!=NULL) {
            defaultAction->mousePressEvent(e);
            e->accept();
        } else {
            e->ignore();
        }
    }
}

/**
 * Called by GV_Scene 
 */
void GV_EventHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {

        currentActions[actionIndex]->mouseReleaseEvent(e);
        // Clean up actions - one might be finished now
        cleanUp();
        e->accept();
    } else {
        if (defaultAction!=NULL) {
            defaultAction->mouseReleaseEvent(e);
        } else {
            e->ignore();
        }
    }
}

/**
 * Handles command line events.
 */
void GV_EventHandler::commandEvent(GV_CommandEvent* e) {
    //RS_RegExp rex;
    RB_String cmd = e->getCommand();

    if (coordinateInputEnabled) {
        if (!e->isAccepted()) {
            // handle absolute cartesian coordinate input:
            if (cmd.contains(',') && cmd.at(0)!='@') {
                if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
                        !currentActions[actionIndex]->isFinished()) {
                    int commaPos = cmd.indexOf(',');
                    bool ok1, ok2;
                    double x = RMath::eval(cmd.left(commaPos), &ok1);
                    double y = RMath::eval(cmd.mid(commaPos+1), &ok2);

                    if (ok1 && ok2) {
                        GV_CoordinateEvent ce(RVector(x,y));
                        currentActions[actionIndex]->coordinateEvent(&ce);
                    } else {
                        if (GV_DIALOGFACTORY!=NULL) {
                            GV_DIALOGFACTORY->commandMessage(
                                "Expression Syntax Error");
                        }
                    }
                    e->accept();
                }
            }
        }

        // handle relative cartesian coordinate input:
        if (!e->isAccepted()) {
            if (cmd.contains(',') && cmd.at(0)=='@') {
                if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
                        !currentActions[actionIndex]->isFinished()) {
                    int commaPos = cmd.indexOf(',');
                    bool ok1, ok2;
                    double x = RMath::eval(cmd.mid(1, commaPos-1), &ok1);
                    double y = RMath::eval(cmd.mid(commaPos+1), &ok2);

                    if (ok1 && ok2) {
                        GV_CoordinateEvent ce(RVector(x,y) +
                                              mDrawing->getRelativeZero());
                        currentActions[actionIndex]->coordinateEvent(&ce);
                    } else {
                        if (GV_DIALOGFACTORY!=NULL) {
                            GV_DIALOGFACTORY->commandMessage(
                                "Expression Syntax Error");
                        }
                    }
                    e->accept();
                }
            }
        }

        // handle absolute polar coordinate input:
        if (!e->isAccepted()) {
            if (cmd.contains('<') && cmd.at(0)!='@') {
                if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
                        !currentActions[actionIndex]->isFinished()) {
                    int commaPos = cmd.indexOf('<');
                    bool ok1, ok2;
                    double r = RMath::eval(cmd.left(commaPos), &ok1);
                    double a = RMath::eval(cmd.mid(commaPos+1), &ok2);

                    if (ok1 && ok2) {
                        RVector pos;
                        pos.setPolar(r,RMath::deg2rad(a));
                        GV_CoordinateEvent ce(pos);
                        currentActions[actionIndex]->coordinateEvent(&ce);
                    } else {
                        if (GV_DIALOGFACTORY!=NULL) {
                            GV_DIALOGFACTORY->commandMessage(
                                "Expression Syntax Error");
                        }
                    }
                    e->accept();
                }
            }
        }

        // handle relative polar coordinate input:
        if (!e->isAccepted()) {
            if (cmd.contains('<') && cmd.at(0)=='@') {
                if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
                        !currentActions[actionIndex]->isFinished()) {
                    int commaPos = cmd.indexOf('<');
                    bool ok1, ok2;
                    double r = RMath::eval(cmd.mid(1, commaPos-1), &ok1);
                    double a = RMath::eval(cmd.mid(commaPos+1), &ok2);

                    if (ok1 && ok2) {
                        RVector pos;
                        pos.setPolar(r,RMath::deg2rad(a));
                        GV_CoordinateEvent ce(pos +
                                              mDrawing->getRelativeZero());
                        currentActions[actionIndex]->coordinateEvent(&ce);
                    } else {
                        if (GV_DIALOGFACTORY!=NULL) {
                            GV_DIALOGFACTORY->commandMessage(
                                "Expression Syntax Error");
                        }
                    }
                    e->accept();
                }
            }
        }
    }

    // send command event directly to current action:
    if (!e->isAccepted()) {
        if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
                !currentActions[actionIndex]->isFinished()) {
            currentActions[actionIndex]->commandEvent(e);
            //e->accept();
        } else {
            if (defaultAction!=NULL) {
                defaultAction->commandEvent(e);
                //e->accept();
            }
        }
    }

    RB_DEBUG->print("GV_EventHandler::commandEvent: OK");
}

/**
 * Handles coordinate events (e.g. from coordinate input widgets).
 */
void GV_EventHandler::coordinateEvent(GV_CoordinateEvent* e) {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {

        currentActions[actionIndex]->coordinateEvent(e);
    }
}

/**
 * Enables coordinate input in the command line.
 */
void GV_EventHandler::enableCoordinateInput() {
    coordinateInputEnabled = true;
}

/**
 * Enables coordinate input in the command line.
 */
void GV_EventHandler::disableCoordinateInput() {
    coordinateInputEnabled = false;
}

/**
 * @return Current action.
 */
GV_ActionBase* GV_EventHandler::getCurrentAction() {
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
            !currentActions[actionIndex]->isFinished()) {

        return currentActions[actionIndex];
    } else {
        return defaultAction;
    }
}

/**
 * @return The current default action.
 */
GV_ActionBase* GV_EventHandler::getDefaultAction() {
    return defaultAction;
}

/**
 * Sets the default action.
 */
void GV_EventHandler::setDefaultAction(GV_ActionBase* action) {
    if (defaultAction!=NULL) {
        defaultAction->finish();
        delete defaultAction;
        defaultAction = NULL;
    }

    defaultAction = action;
}

/**
 * Sets the current action, called by GV_Scene
 */
void GV_EventHandler::setCurrentAction(GV_ActionBase* action) {
    RB_DEBUG->print("GV_EventHandler::setCurrentAction");
    
    if (action==NULL) {
        return;
    }

    if (action->isExclusive()) {
        killAllActions();
    }

    // Predecessor of the new action or NULL:
    GV_ActionBase* predecessor = NULL;

    // Suspend current action:
    if (actionIndex>=0 && currentActions[actionIndex]!=NULL &&
        !currentActions[actionIndex]->isFinished()) {

        predecessor = currentActions[actionIndex];
        predecessor->suspend();
//        predecessor->hideOptions();
    } else {
        if (defaultAction!=NULL) {
            predecessor = defaultAction;
            predecessor->suspend();
//            predecessor->hideOptions();
        }
    }

    // Forget about the oldest action and make space for the new action:
    if (actionIndex==GV_MAXACTIONS-1) {
        // delete oldest action if necessary (usually never happens):
        if (currentActions[0]!=NULL) {
            currentActions[0]->finish();
            delete currentActions[0];
            currentActions[0] = NULL;
        }
        // Move up actionstack (optimize):
        for (int i=0; i<GV_MAXACTIONS-1; ++i) {
            currentActions[i] = currentActions[i+1];
        }
    } else if (actionIndex<GV_MAXACTIONS-1) {
        actionIndex++;
    }

    // Set current action:
    currentActions[actionIndex] = action;

    // Initialisation of our new action:
    RB_DEBUG->print("GV_EventHandler::setCurrentAction: init current action");
    action->init();
    action->init(0);

    if (action->isFinished()==false) {
        RB_DEBUG->print("GV_EventHandler::setCurrentAction: show options");
//        currentActions[actionIndex]->showOptions();
        RB_DEBUG->print("GV_EventHandler::setCurrentAction: set predecessor");
        action->setPredecessor(predecessor);
    }

    RB_DEBUG->print("GV_EventHandler::setCurrentAction: cleaning up..");
    cleanUp();

    RB_DEBUG->print("GV_EventHandler::setCurrentAction: debugging actions");
//	debugActions();
    RB_DEBUG->print("GV_EventHandler::setCurrentAction: OK");
}

/**
 * Kills all running selection actions. Called when a selection action
 * is launched to reduce confusion.
 */
void GV_EventHandler::killSelectActions() {
    RB_DEBUG->print("GV_EventHandler::killSelectActions");
    for (int c=0; c<GV_MAXACTIONS; ++c) {
        if (currentActions[c]!=NULL) {
//			if (dynamic_cast<GV_ActionSelectBase*>(currentActions[c])!=NULL) {
				currentActions[c]->finish();
//			}
        }
    }
}

/**
 * Kills all running actions.
 */
void GV_EventHandler::killAllActions() {
   for (int c=0; c<GV_MAXACTIONS; ++c) {
       if (currentActions[c]!=NULL && !currentActions[c]->isPersistent()) {
           currentActions[c]->finish();
       }
   }
   actionIndex = -1;
}

/**
 * @return true if there is at least one action in the action stack.
 */
bool GV_EventHandler::hasAction() {
    if (actionIndex!=-1 || defaultAction!=NULL) {
        return true;
    } else {
        return false;
    }
}

/**
 * Garbage collector for actions.
 */
void GV_EventHandler::cleanUp() {
    RB_DEBUG->print("GV_EventHandler::cleanUp");

    int o=0;   // old index
    int n=0;   // new index
    int resume=0; // index of action to resume
    bool doResume=false; // do we need to resume an action
    actionIndex = -1;

    //debugActions();
    do {
        // search first used action (o)
        while (currentActions[o]==NULL && o<GV_MAXACTIONS) {
            o++;
        }

        // delete action if it is finished
        if (o<GV_MAXACTIONS && currentActions[o]!=NULL &&
            currentActions[o]->isFinished()) {
            delete currentActions[o];
            currentActions[o] = NULL;

            doResume = true;
        }

        // move a running action up in the stack
        if (o<GV_MAXACTIONS && currentActions[o]!=NULL) {
            if (n!=o) {
                currentActions[n] = currentActions[o];
                resume = n;
                currentActions[o] = NULL;
            } else {
                if (o<GV_MAXACTIONS) {
                    o++;
                }
            }
            actionIndex = n;
            if (n<GV_MAXACTIONS-1) {
                n++;
            }
        }
    } while (o<GV_MAXACTIONS);
        
    //debugActions();

    // Resume last used action:
    if (doResume) {
        if (currentActions[resume]!=NULL &&
            !currentActions[resume]->isFinished()) {
        
            currentActions[resume]->resume();
//            currentActions[resume]->showOptions();
        } else {
            if (defaultAction!=NULL) {
                defaultAction->resume();
//                defaultAction->showOptions();
            }
        }
    }
    RB_DEBUG->print("GV_EventHandler::cleanUp: OK");
}

/**
 * Sets the snap mode for all currently active actions.
 */
void GV_EventHandler::setSnapMode(GV2::ActionType sm) {
    for (int c=0; c<GV_MAXACTIONS; ++c) {
        if (currentActions[c]!=NULL) {
            currentActions[c]->setSnapMode(sm);
        }
    }

    if (defaultAction!=NULL) {
        defaultAction->setSnapMode(sm);
    }
}

/**
 * Sets the snap restriction for all currently active actions.
 */
void GV_EventHandler::setSnapRestriction(GV2::ActionType sr) {
    for (int c=0; c<GV_MAXACTIONS; ++c) {
        if (currentActions[c]!=NULL) {
            currentActions[c]->setSnapRestriction(sr);
        }
    }

    if (defaultAction!=NULL) {
        defaultAction->setSnapRestriction(sr);
    }
}

void GV_EventHandler::debugActions() {
    RB_DEBUG->print("---");
    for (int c=0; c<GV_MAXACTIONS; ++c) {
        if (c==actionIndex) {
            RB_DEBUG->print("Current");
        }
        if (currentActions[c]!=NULL) {
            RB_DEBUG->print("Action %03d: %s [%s]",
                            c, 
                            (const char*)currentActions[c]->name().toLatin1(),
                            currentActions[c]->isFinished() ? "finished" : "active");
        } else {
            RB_DEBUG->print("Action %03d: NULL", c);
        }
    }
}

