/*****************************************************************
 * $Id: gv_actionhelpmanual.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Jan 23, 2009 3:20:12 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionhelpmanual.h"
#include "main.h"

/**
 * Constructor.
 */
GV_ActionHelpManual::GV_ActionHelpManual(GV_Drawing& dwg, GV_GraphicsView& gv) 
                                            : GV_ActionBase(dwg, gv) {
}


RB_GuiAction* GV_ActionHelpManual::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionHelpManual::getName(),
        			"helpmanual",
                    tr("&Manual"));
        action->setStatusTip(tr("Launch online manual"));
        action->setShortcut(Qt::Key_F1);
    }
    return action;
}
