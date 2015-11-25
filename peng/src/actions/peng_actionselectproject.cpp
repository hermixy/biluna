/*****************************************************************
 * $Id: peng_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 16, 2011 21:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_actionselectproject.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "peng_dialogfactory.h"
#include "peng_projectdialog.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


PENG_ActionSelectProject::PENG_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PENG_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PENG_ActionSelectProject::getName(),
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
        ga->setFactory(PENG_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* PENG_ActionSelectProject::factory() {
    RB_Action* a = new PENG_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger(); by conditionalPlugin()

    const QString pluginType = "PENG";
    bool result = DB_PERMISSIONHANDLER->conditionalPlugin(a, pluginType);
    delete a;
    a = nullptr;

    if (!result) {
        // Close plugin if was already opened by previous user
        DB_ACTIONFACTORY->closePlugin(pluginType);
    }

    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void PENG_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("PENG", 0, 9, 14)) {
            PENG_DIALOGFACTORY->requestWarningDialog(tr("PENG check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            PENG_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(PENG_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            // Add default rows if none exist
            PENG_MODELFACTORY->addDefaultRows();
        } else  {
            PENG_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        PENG_DIALOGFACTORY->deleteAllMdiWindows();
        PENG_DIALOGFACTORY->deleteAllDockWidgets();

        if (PENG_MODELFACTORY->getRootId() != "") {
            PENG_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready.\n"
                                                       "Select action from perspective menu"));
            PENG_DIALOGFACTORY->commandMessage(tr("PENG perspective is ready"));
            PENG_DIALOGFACTORY->statusBarMessage(tr("PENG perspective is ready"), 2000);
        }

    } else {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    PENG_MODELFACTORY->emitState();
}
