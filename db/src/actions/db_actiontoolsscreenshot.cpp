/*****************************************************************
 * $Id: db_actiontoolsscreenshot.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Feb 23, 2015 9:38:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolsscreenshot.h"

#include "db_dialogfactory.h"


DB_ActionToolsScreenshot::DB_ActionToolsScreenshot()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionToolsScreenshot::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionToolsScreenshot::getName(),
//                                  "dialog",
                                  tr("Screensh&ot"));
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database ..."));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionToolsScreenshot::factory);
    }
    return ga;
}

RB_Action* DB_ActionToolsScreenshot::factory() {
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogScreenshot);

    // Modal
    // dlg->exec();
    // dlg->deleteLater();

    // Modeless, delete on close or by deletion of mainwindow (is parent widget)
    dlg->show();
    dlg->raise();
    dlg->activateWindow();

    return NULL;
}
