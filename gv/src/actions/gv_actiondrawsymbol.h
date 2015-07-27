/*****************************************************************
 * $Id: gv_actiondrawsymbol.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 16, 2008 1:20:23 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionblockadd.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONDRAWSYMBOL_H
#define GV_ACTIONDRAWSYMBOL_H

#include "gv_actionbase.h"

/**
 * This action class can handle user events to draw symbols.
 */
class GV_ActionDrawSymbol : public GV_ActionBase {

  Q_OBJECT

public:
    //! Action States.
    enum Status {
        SetTargetPoint,    /**< Setting the reference point. */
        SetAngle,          /**< Setting angle in the command line. */
        SetFactor,         /**< Setting factor in the command line. */
        SetColumns,        /**< Setting columns in the command line. */
        SetRows,           /**< Setting rows in the command line. */
        SetColumnSpacing,  /**< Setting column spacing in the command line. */
        SetRowSpacing      /**< Setting row spacing in the command line. */
    };

public:
    GV_ActionDrawSymbol(GV_Drawing& dwg, GV_GraphicsView& gv);
	~GV_ActionDrawSymbol();

    static RB_String getName() {
        return "Draw Symbol";
    }

    virtual RB_String name() {
        return GV_ActionDrawSymbol::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
	virtual void dropEvent(QGraphicsSceneDragDropEvent* e);
    virtual void coordinateEvent(GV_CoordinateEvent* e);
    virtual void commandEvent(GV_CommandEvent* e);
    virtual RB_StringList getAvailableCommands();

//    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
//    virtual void updateToolBar();
//    

protected:
    GV_Symbol* mLegendSymbol;
    GV_Symbol* mNewSymbol;
	RVector mInsertionPoint;
    
    /** Last status before entering option. */
    Status lastStatus;
};

#endif
