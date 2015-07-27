/*****************************************************************
 * $Id: gv_actionfilesave.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:33:04 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionfilesave.cpp 8821
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfilesave.h"


GV_ActionFileSave::GV_ActionFileSave(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {}


RB_GuiAction* GV_ActionFileSave::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionFileSave::getName(),
        						"filesave",
                                tr("&Save"));
        action->setShortcut("Ctrl+S");
        action->setStatusTip(tr("Saves the current GV project"));
    }                                  
    return action;
}
