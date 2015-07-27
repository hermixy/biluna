/*****************************************************************
 * $Id: gv_actionfilequit.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:29:13 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actionfilequit.cpp 4269
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfilequit.h"



GV_ActionFileQuit::GV_ActionFileQuit(GV_Drawing& dwg, GV_GraphicsView& gv)
                :GV_ActionBase(dwg, gv) {}


RB_GuiAction* GV_ActionFileQuit::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionFileQuit::getName(), /*QIcon("exit.png"),*/
                            tr("&Quit"));
        action->setShortcut("Ctrl+Q");
        action->setStatusTip(tr("Quits the application"));
    }                                  
    return action;
}
