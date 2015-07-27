/*****************************************************************
 * $Id: gv_actiondrawline.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 5, 2008 4:00:51 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actiondrawline.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONDRAWLINE_H
#define GV_ACTIONDRAWLINE_H

#include "gv_actionbase.h"
#include "gv_line.h"
#include "rb_list.h"

#include <qaction.h>


/**
 * This action class can handle user events to draw 
 * simple lines with the start- and endpoint given.
 */
class GV_ActionDrawLine : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    /**
     * Action States.
     */
    enum Status {
        SetStartpoint,   /**< Setting the startpoint.  */
        SetEndpoint      /**< Setting the endpoint. */
    };

public:
    GV_ActionDrawLine(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionDrawLine();

    static RB_String getName() {
        return "Draw Line";
    }

    virtual RB_String name() {
        return GV_ActionDrawLine::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    
    void reset();

    virtual void init(int status=0);
    virtual void trigger();
    
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
	virtual void dropEvent(QGraphicsSceneDragDropEvent* e);
    
	virtual void coordinateEvent(GV_CoordinateEvent* ce);
    virtual void commandEvent(GV_CommandEvent* e);
    virtual RB_StringList getAvailableCommands();
    
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

    void close();
    void undo();

protected:
    /**
    * Line data defined so far.
    */
    GV_Line* mNewLine;
    /**
     * Start point of the series of lines. Used for close function.
     */
    RVector start;

    /**
     * Point history (for undo)
     */
    RB_List<RVector> history;
    
};

#endif
