/*****************************************************************
 * $Id: gv_actionfileopen.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 7:06:27 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfileopen.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILEOPEN_H
#define GV_ACTIONFILEOPEN_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to open a project.
 */
class GV_ActionFileOpen : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionFileOpen(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionFileOpen() {}

    static RB_String getName() {
        return "File Open";
    }

    virtual RB_String name() {
        return GV_ActionFileOpen::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
