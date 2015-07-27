/*****************************************************************
 * $Id: gv_actionviewstatusbar.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 12:38:17 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionviewstatusbar.h ?
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONVIEWSTATUSBAR_H
#define GV_ACTIONVIEWSTATUSBAR_H

#include "gv_actionbase.h"


/**
 * This action views or hides the status bar.
 */
class GV_ActionViewStatusBar : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionViewStatusBar(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionViewStatusBar() {}

    static RB_String getName() {
        return "View Status Bar";
    }

    virtual RB_String name() {
        return GV_ActionViewStatusBar::getName();
    }

    static RB_GuiAction* createGuiAction();
//    static RB_Action* factory();
};

#endif
