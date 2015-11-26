/*****************************************************************
 * $Id: scan_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 26, 2011 21:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_actionselectproject.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"
#include "scan_dialogfactory.h"
#include "scan_projectdialog.h"


SCAN_ActionSelectProject::SCAN_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SCAN_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SCAN_ActionSelectProject::getName(),
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
        ga->setFactory(SCAN_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* SCAN_ActionSelectProject::factory() {
    RB_Action* a = new SCAN_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger(); by conditionalPlugin()

    const QString pluginType = "SCAN";
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
void SCAN_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("SCAN", 0, 9, 13)) {
            SCAN_DIALOGFACTORY->requestWarningDialog(tr("SCAN check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = SCAN_DIALOGFACTORY->getDialog(SCAN_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();
            if (!obj) {
                dlg->deleteLater();
                return;
            }
            RB_String id = obj->getId();
            // Set root id of all models
            SCAN_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = SCAN_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(SCAN_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        } else  {
            SCAN_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        SCAN_DIALOGFACTORY->deleteAllMdiWindows();
        SCAN_DIALOGFACTORY->deleteAllDockWidgets();

        if (SCAN_MODELFACTORY->getRootId() != "") {
            SCAN_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready.\n"
                                                       "Select action from perspective menu"));
        }

    } else {
        SCAN_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    SCAN_MODELFACTORY->emitState();
}
