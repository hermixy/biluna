/*****************************************************************
 * $Id: gv_actionsetsnapmode.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 23, 2008 2:14:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmode.h 10343 
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSETSNAPMODE_H
#define GV_ACTIONSETSNAPMODE_H

#include "gv_actionbase.h"


/**
 * This action changes the current snap mode.
 */
class GV_ActionSetSnapMode : public GV_ActionBase {
	
public:
    GV_ActionSetSnapMode(GV_Drawing& dwg, GV_GraphicsView& gv,
                         GV2::ActionType snapMode);
    ~GV_ActionSetSnapMode() {}

    /*
    static GV_String getName() {
        return "Set Snap Mode";
    }

    virtual GV_String name() {
        return GV_ActionSetSnapMode::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    static void updateGuiActions(GV2::ActionType snapMode);
    static GV2::ActionType getSnapMode();
    */

    virtual void init(int status=0);
    virtual void trigger();

protected:
    GV2::ActionType snapMode;

    //static GV_Hash<RS2::ActionType, RB_GuiAction*> actions;
    static QActionGroup* actionGroup;
};

#endif
