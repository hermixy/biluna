/*****************************************************************
 * $Id: gv_actionviewgrid.h 1148 2010-05-08 11:53:42Z rutger $
 * Created: Oct 5, 2008 12:34:43 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actionviewgrid.h ?
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONVIEWGRID_H
#define GV_ACTIONVIEWGRID_H

#include "gv_actionbase.h"


/**
 * This action toggles the grid visibility (local to drawing).
 */
class GV_ActionViewGrid : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionViewGrid(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionViewGrid() {}

    static RB_String getName() {
        return "View Grid";
    }

    virtual RB_String name() {
        return GV_ActionViewGrid::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual void init(int status=0);
    virtual void trigger();
    
private:
    static RB_GuiAction* mGuiAction;
};

#endif
