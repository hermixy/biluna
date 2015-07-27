/*****************************************************************
 * $Id: gv_actionfileclose.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 12, 2008 2:10:30 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfileclose.h"

GV_ActionFileClose::GV_ActionFileClose(GV_Drawing& dwg, GV_GraphicsView& gv)
                : GV_ActionBase(dwg, gv) { }

RB_GuiAction* GV_ActionFileClose::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionFileClose::getName(),
        				"fileclose",
                        tr("&Close Project"));
        action->setStatusTip(tr("Closes this project and all drawings"));
        action->setShortcut("Ctrl+Shift+Q");
    }                         
    return action;
}


