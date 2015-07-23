/*****************************************************************
 * $Id: db_actionsystemtoolsregexp.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Oct 22, 2012 7:38:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsregexp.h"

#include "db_dialogfactory.h"



DB_ActionSystemToolsRegExp::DB_ActionSystemToolsRegExp()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemToolsRegExp::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemToolsRegExp::getName(),
//                                  "dialog",
                                  tr("Regular E&xpression"));
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
        ga->setFactory(DB_ActionSystemToolsRegExp::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemToolsRegExp::factory() {
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogRegExp);

    // Modal
    // dlg->exec();

    // Modeless, delete on close or by deletion of mainwindow (is parent widget)
    dlg->show();
    dlg->raise();
    dlg->activateWindow();

    return NULL;
}
