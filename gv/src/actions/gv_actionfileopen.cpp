/*****************************************************************
 * $Id: gv_actionfileopen.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 7:08:36 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfileopen.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfileopen.h"


GV_ActionFileOpen::GV_ActionFileOpen(GV_Drawing& dwg, GV_GraphicsView& gv)
                    :GV_ActionBase(dwg, gv) {}


RB_GuiAction* GV_ActionFileOpen::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionFileOpen::getName(),
        						"fileopen",
                                tr("&Open..."));
        action->setShortcut("Ctrl+O");
        action->setStatusTip(tr("Opens an existing GV project"));
    }                                  
    return action;
}


