/*****************************************************************
 * $Id: gv_eventhandler.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 7, 2008 7:59:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_eventhandler 8364
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_EVENTHANDLER_H
#define GV_EVENTHANDLER_H

#include <QtWidgets>
#include "gv_actionbase.h"
#include "gv_commandevent.h"
#include "gv_coordinateevent.h"

// #include "rs_keyevent.h"
// #include "rs_mouseevent.h"

#define GV_MAXACTIONS 16


/**
 * The event handler owns and manages all actions that are currently 
 * active. All events going from the view scene to the actions pass this class.
 */
class GV_EventHandler {
	
public:
    GV_EventHandler(GV_Drawing* dwg);
    ~GV_EventHandler();

    void back();
    void enter();

    void dragEnterEvent(QGraphicsSceneDragDropEvent* e);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* e);
    void dragMoveEvent(QGraphicsSceneDragDropEvent* e);
    void dropEvent(QGraphicsSceneDragDropEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    void mouseEnterEvent();
    void mouseLeaveEvent();

    void commandEvent(GV_CommandEvent* e);
    void coordinateEvent(GV_CoordinateEvent* e);
    void enableCoordinateInput();
    void disableCoordinateInput();

    void setDefaultAction(GV_ActionBase* action);
    GV_ActionBase* getDefaultAction();
    
    void setCurrentAction(GV_ActionBase* action);
    GV_ActionBase* getCurrentAction();
    
    void killSelectActions();
    void killAllActions();
    
    bool hasAction();
    void cleanUp();
    void debugActions();
    void setSnapMode(GV2::ActionType sm);
    void setSnapRestriction(GV2::ActionType sr);

protected:
    GV_Drawing* mDrawing;
    GV_ActionBase* defaultAction;
    GV_ActionBase* currentActions[GV_MAXACTIONS];
    int actionIndex;
    bool coordinateInputEnabled;
};

#endif
