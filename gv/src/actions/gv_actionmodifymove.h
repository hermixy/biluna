/*****************************************************************
 * $Id$
 * Created: Nov 12, 2008 1:16:48 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionmodifymove.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONMODIFYMOVE_H
#define GV_ACTIONMODIFYMOVE_H

#include "gv_actionbase.h"
// #include "rs_modification.h"


/**
 * This action class can handle user events to move entities.
 */
class GV_ActionModifyMove : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    /**
     * Action States.
     */
//    enum Status {
//        SetReferencePoint,    /**< Setting the reference point. */
//        SetTargetPoint,       /**< Setting the target point. */
//        ShowDialog            /**< Showing the options dialog. */
//    };

public:
    GV_ActionModifyMove(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionModifyMove() {}

    static RB_String getName() {
        return "Modify Move";
    }

    virtual RB_String name() {
        return GV_ActionModifyMove::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    
    virtual bool isExclusive() {
        return true;
    }

    virtual void init(int status=0);
    
    // virtual void trigger();
    
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    virtual void coordinateEvent(GV_CoordinateEvent* e);

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
    
protected:

private:
//    RS_MoveData data;
    RVector referencePoint;
    RVector targetPoint;

};

#endif
