/*****************************************************************
 * $Id: db_actiontoolssignalspy.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolssignalspy.h"


DB_ActionToolsSignalSpy::DB_ActionToolsSignalSpy() : RB_Action() {

}


RB_GuiAction* DB_ActionToolsSignalSpy::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionToolsSignalSpy::getName(),
                                  "fileclose",
                                  tr("&Signal Spy"));
        action->setStatusTip(tr("Monitoring signal/slot communication between QObject's"));
//        action->setShortcut("Ctrl+W");
    }                                  
    return action;
}

