/*****************************************************************
 * $Id: peng_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_actionselectproject.h"

#include "db_modelfactory.h"
#include "db_actionfactory.h"
#include "pcalc_dialogfactory.h"
#include "pcalc_projectdialog.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


PCALC_ActionSelectProject::PCALC_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* PCALC_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(PCALC_ActionSelectProject::getName(),
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
        ga->setFactory(PCALC_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* PCALC_ActionSelectProject::factory() {
    RB_Action* a = new PCALC_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger(); by conditionalPlugin()

    const QString pluginType = "PCALC";
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
void PCALC_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("PCALC", 0, 9, 15)) {
            PCALC_DIALOGFACTORY->requestWarningDialog(
                        tr("PCALC check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = PCALC_DIALOGFACTORY->getDialog(PCALC_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            PCALC_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = PCALC_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(PCALC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            // Add default rows if none exist
            PCALC_MODELFACTORY->addDefaultRows();
        } else  {
            PCALC_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        PCALC_DIALOGFACTORY->deleteAllMdiWindows();
        PCALC_DIALOGFACTORY->deleteAllDockWidgets();

        if (PCALC_MODELFACTORY->getRootId() != "") {
            PCALC_DIALOGFACTORY->requestWarningDialog(
                        tr("Perspective is ready.\n"
                           "Select action from perspective menu"));
            PCALC_DIALOGFACTORY->commandMessage(
                        tr("PCALC perspective is ready"));
            PCALC_DIALOGFACTORY->statusBarMessage(
                        tr("PCALC perspective is ready"), 2000);
        }

    } else {
        PCALC_DIALOGFACTORY->requestWarningDialog(
                    tr("Not connected to database."));
    }

    PCALC_MODELFACTORY->emitState();
}
