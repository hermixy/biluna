/*****************************************************************
 * $Id: db_actionsystemtoolssignalspy.cpp 1481 2011-11-21 19:28:26Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolssignalspy.h"
#include "db_dialogfactory.h"
#include "rb_mainwindow.h"


DB_ActionSystemToolsSignalSpy::DB_ActionSystemToolsSignalSpy() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemToolsSignalSpy::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionSystemToolsSignalSpy::getName(),
                                  // "fileclose",
                                  tr("&Signal Spy"));
        ga->setStatusTip(tr("Monitoring signal/slot communication between QObject's"));
//        ga->setShortcut("Ctrl+W");
        ga->setFactory(DB_ActionSystemToolsSignalSpy::factory);
    }

    return ga;
}

RB_Action* DB_ActionSystemToolsSignalSpy::factory() {
    RB_Action* a = new DB_ActionSystemToolsSignalSpy();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemToolsSignalSpy::trigger() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotDebugSignalSpy();
}

