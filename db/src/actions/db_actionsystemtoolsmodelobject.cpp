/*****************************************************************
 * $Id: db_actiontoolsmodelobject.cpp 2248 2015-12-28 14:13:00Z rutger $
 * Created: Dec 28, 2015 14:38:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsmodelobject.h"

#include "db_dialogfactory.h"


DB_ActionSystemToolsModelObject::DB_ActionSystemToolsModelObject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemToolsModelObject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemToolsModelObject::getName(),
//                                  "dialog",
                                  tr("&Model object"));
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
        ga->setFactory(DB_ActionSystemToolsModelObject::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemToolsModelObject::factory() {
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogModelObject);

    // Modal
    // dlg->exec();
    // dlg->deleteLater();

    // Modeless, delete on close or by deletion of mainwindow (is parent widget)
    dlg->show();
    dlg->raise();
    dlg->activateWindow();

    return NULL;
}
