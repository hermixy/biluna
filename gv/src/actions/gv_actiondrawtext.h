/*****************************************************************
 * $Id: gv_actiondrawtext.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 16, 2008 1:20:23 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionblockadd.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONDRAWTEXT_H
#define GV_ACTIONDRAWTEXT_H

#include "gv_actionbase.h"

/**
 * This action class can handle user events to draw texts.
 */
class GV_ActionDrawText : public GV_ActionBase {

  Q_OBJECT

public:
    GV_ActionDrawText(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionDrawText();

    static RB_String getName() {
        return "Draw Text";
    }

    virtual RB_String name() {
        return GV_ActionDrawText::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual void init(int status=0);
    virtual void trigger();

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
//	virtual void dropEvent(QGraphicsSceneDragDropEvent* e);
    virtual void coordinateEvent(GV_CoordinateEvent* e);
    virtual void commandEvent(GV_CommandEvent* e);
    virtual RB_StringList getAvailableCommands();

//    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
//    virtual void updateToolBar();
//

protected:
    GV_Text* mLegendText;
//    GV_Symbol* mNewSymbol;
    RVector mInsertionPoint;

    /** Last status before entering option. */
//    Status lastStatus;
};

#endif // GV_ACTIONDRAWTEXT_H
