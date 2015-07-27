/*****************************************************************
 * $Id: gv_actionfileprint.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:24:27 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actionfileprint.cpp 4269
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfileprint.h"


GV_ActionFilePrint::GV_ActionFilePrint(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {}


RB_GuiAction* GV_ActionFilePrint::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionFilePrint::getName(),
            "fileprint",
                             tr("&Print..."));
        action->setShortcut("Ctrl+P");
        action->setStatusTip(tr("Prints out the current document"));
    }                                  
    return action;
}


