/*****************************************************************
 * $Id: gv_actionhelpwebsite.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Jan 23, 2009 3:32:53 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionhelpwebsite.h"
#include "main.h"

/**
 * Constructor.
 */
GV_ActionHelpWebsite::GV_ActionHelpWebsite(GV_Drawing& dwg, GV_GraphicsView& gv) 
                                            : GV_ActionBase(dwg, gv) {
}


RB_GuiAction* GV_ActionHelpWebsite::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionHelpWebsite::getName(),
        			"website",
                    tr("Visit &Website"));
        action->setStatusTip(tr("Visit the website"));
//        action->setShortcut(Qt::Key_F1);
    }
    return action;
}
