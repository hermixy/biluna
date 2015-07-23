/*****************************************************************
 * $Id: rb_actiondebugexportmodel.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 16, 2009 12:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_actiondebugexportmodel.h"

/**
 * Constructor
 */
RB_ActionDebugExportModel::RB_ActionDebugExportModel() : RB_Action() {

}


RB_GuiAction* RB_ActionDebugExportModel::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(RB_ActionDebugExportModel::getName(),
//                                  "debugexportmodel",
                                  tr("E&xport Model"));
        action->setStatusTip(tr("Debug model"));
    }
    return action;
}
