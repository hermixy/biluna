/*****************************************************************
 * $Id: gv_actionviewstatusbar.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 12:36:40 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionviewstatusbar.cpp 3703
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionviewstatusbar.h"

//#include <QPixmap>

#include "rb_settings.h"


/**
 * Constructor.
 */
GV_ActionViewStatusBar::GV_ActionViewStatusBar(GV_Drawing& dwg, 
            GV_GraphicsView& gv) : GV_ActionBase(dwg, gv) {

}


RB_GuiAction* GV_ActionViewStatusBar::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionViewStatusBar::getName(), tr("&Statusbar"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setStatusTip(tr("Enables/disables the statusbar"));
    }

    return action;
}


