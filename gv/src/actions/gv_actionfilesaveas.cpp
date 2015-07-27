/*****************************************************************
 * $Id: gv_actionfilesaveas.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 2, 2008 8:36:20 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionnfilesaveas.cpp 8281
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionfilesaveas.h"


GV_ActionFileSaveAs::GV_ActionFileSaveAs(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {}


RB_GuiAction* GV_ActionFileSaveAs::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionFileSaveAs::getName(),
            "filesaveas", 
            tr("Save &as..."));
        action->setShortcut("Ctrl+Shift+S");
        action->setStatusTip(tr("Saves the current GV project under"
                                " a new filename"));
        
    }
    
    return action;
}

// EOF
