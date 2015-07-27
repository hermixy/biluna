/*****************************************************************
 * $Id: gv_actionsetsnapmodegrid.h 1267 2010-08-02 16:14:33Z rutger $
 * Created: Oct 23, 2008 2:25:58 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmodegrid.h 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSETSNAPMODEGRID_H
#define GV_ACTIONSETSNAPMODEGRID_H

#include "gv_actionsetsnapmode.h"


/**
 * Sets the current snap mode to grid snap.
 */
class GV_ActionSetSnapModeGrid : public GV_ActionSetSnapMode {
    Q_OBJECT

public:
    GV_ActionSetSnapModeGrid(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionSetSnapModeGrid() {}

    static RB_String getName() {
        return "Set Snap Mode Grid";
    }

    virtual RB_String name() {
        return GV_ActionSetSnapModeGrid::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

protected:
    static RB_GuiAction* action;
};

#endif
