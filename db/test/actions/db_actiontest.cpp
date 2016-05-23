/*****************************************************************
 * $Id: db_actiontest.cpp 1031 2009-11-04 17:56:54Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontest.h"

DB_ActionTest::DB_ActionTest() {
    // nothing
}

RB_GuiAction* DB_ActionTest::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionTest::getName(),
                                  /*QIcon("test.png"),*/
                                  tr("&Action"));
        action->setShortcut("Ctrl+A");
        action->setToolTip(tr("Tool tip: Test of the actions"));
        action->setStatusTip(tr("Status tip:Test of the actions"));
    }
    return action;
}
