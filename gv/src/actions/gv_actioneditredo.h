/*****************************************************************
 * $Id: gv_actioneditredo.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 4, 2008 8:51:50 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rsactioneditredo.h 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONEDITREDO_H
#define GV_ACTIONEDITREDO_H

#include "gv_actioneditundo.h"

/**
 * This action class can handle user events for redo.
 */
class GV_ActionEditRedo : public GV_ActionEditUndo {
	
    Q_OBJECT
    
public:
    GV_ActionEditRedo(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionEditRedo();

    static RB_String getName() {
        return "Edit Redo";
    }

    virtual RB_String name() {
        return GV_ActionEditRedo::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
};

#endif
