/*****************************************************************
 * $Id: gv_actionhelpabout.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Jan 23, 2009 2:51:45 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionhelpabout.h"
#include "main.h"

/**
 * Constructor.
 */
GV_ActionHelpAbout::GV_ActionHelpAbout(GV_Drawing& dwg, GV_GraphicsView& gv) 
                                            : GV_ActionBase(dwg, gv) {
}


RB_GuiAction* GV_ActionHelpAbout::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionHelpAbout::getName(),
        			"helpabout",
                    tr("&About %1").arg(RB_APPNAME));
        action->setStatusTip(tr("About the application"));
//        action->setShortcut("Ctrl+H");
    }
    return action;
}
