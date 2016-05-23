/*****************************************************************
 * $Id: db_actiontoolsexportmodel.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolsexportmodel.h"


DB_ActionToolsExportModel::DB_ActionToolsExportModel() : RB_Action() {

}


RB_GuiAction* DB_ActionToolsExportModel::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionToolsExportModel::getName(),
                                  "fileclose",
                                  tr("E&xport Model"));
        action->setStatusTip(tr("Export the current model to the text window"));
//        action->setShortcut("Ctrl+W");
    }                                  
    return action;
}

