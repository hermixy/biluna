/*****************************************************************
 * $Id: gv_actionsetsnaprestriction.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Nov 7, 2008 4:30:24 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnaprestrion.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONSETSNAPRESTRICTION_H
#define GV_ACTIONSETSNAPRESTRICTION_H

#include "gv_actionbase.h"
    
    

/**
 * This action changes the current additional snap mode.
 */
class GV_ActionSetSnapRestriction : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionSetSnapRestriction(GV_Drawing& dwg, GV_GraphicsView& gv,
                         GV2::ActionType snapRes);
    ~GV_ActionSetSnapRestriction() {}

    static RB_String getName() {
        return "Set Snap Restriction";
    }

    virtual RB_String name() {
        return GV_ActionSetSnapRestriction::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    //static void updateGuiActions(RS2::ActionType snapRes);
    //static RS2::ActionType getSnapRestriction();

    virtual void init(int status=0);
    virtual void trigger();

protected:
    GV2::ActionType snapRes;
    static QActionGroup* actionGroup;

//private:
    //static RS_Hash<RS2::ActionType, RS_GuiAction*> actions;
};

#endif
