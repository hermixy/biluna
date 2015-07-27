/*****************************************************************
 * $Id: gv_actionsetsnapmodeauto.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 23, 2008 2:19:29 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmodeauto.h 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSETSNAPMODEAUTO_H
#define GV_ACTIONSETSNAPMODEAUTO_H

#include "gv_actionsetsnapmode.h"


/**
 * Sets the current snap mode to auto snap.
 */
class GV_ActionSetSnapModeAuto : public GV_ActionSetSnapMode {
	
    Q_OBJECT

public:
    GV_ActionSetSnapModeAuto(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionSetSnapModeAuto() {}

    static RB_String getName() {
        return "Set Snap Mode Auto";
    }

    virtual RB_String name() {
        return GV_ActionSetSnapModeAuto::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

protected:
    static RB_GuiAction* action;
};

#endif
