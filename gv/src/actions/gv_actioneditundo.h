/*****************************************************************
 * $Id: gv_actioneditundo.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:56:58 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actioneditundo.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITUNDO_H
#define GV_ACTIONEDITUNDO_H

#include "gv_actionbase.h"
// #include "rs_undo.h" TODO: how about undo from Qt?

/**
 * This action class can handle user events for undo / redo.
 */
class GV_ActionEditUndo : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionEditUndo(GV_Drawing& dwg, GV_GraphicsView& gv,
                      bool undo);
    ~GV_ActionEditUndo();

    static RB_String getName() {
        return "Edit Undo";
    }

    virtual RB_String name() {
        return GV_ActionEditUndo::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
/*
    virtual void init(int status=0);
    virtual void trigger();
*/
protected:
    /** Undo (true) or redo (false) */
    bool undo;
};

#endif
