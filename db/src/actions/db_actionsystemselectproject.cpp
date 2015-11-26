/*****************************************************************
 * $Id: db_actionsystemselectproject.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Dec 8, 2009 21:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemselectproject.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"
#include "db_projectdialog.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


DB_ActionSystemSelectProject::DB_ActionSystemSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemSelectProject::getName(),
                                  "dialog",
                                  tr("&Select project"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionSystemSelectProject::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemSelectProject::factory() {
    RB_Action* a = new DB_ActionSystemSelectProject();
    // no graphicsView with eventhandler which deletes the action
    DB_PERMISSIONHANDLER->conditionalExecute(a); // a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Open project selection dialog
        RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();
            if (!obj) {
                dlg->deleteLater();
                return;
            }
            RB_String id = obj->getId();
            // Set all open perspective root ID's to invalid ID
            DB_MODELFACTORY->setAllRootId("");
            // Set root id
            DB_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = DB_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(DB_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        }

        dlg->deleteLater();
        DB_DIALOGFACTORY->deleteAllMdiWindows();
        DB_DIALOGFACTORY->deleteAllDockWidgets();

        if (DB_MODELFACTORY->getRootId() != "") {
            DB_DIALOGFACTORY->commandMessage(tr("DB project selected"));
        } else {
            DB_DIALOGFACTORY->commandMessage(
                        tr("Not a valid DB project selected"));
        }
    } else {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    DB_MODELFACTORY->emitState();
}
