/*****************************************************************
 * $Id: sail_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_actionselectproject.h"

#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"
#include "sail_dialogfactory.h"
#include "sail_projectdialog.h"


SAIL_ActionSelectProject::SAIL_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SAIL_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SAIL_ActionSelectProject::getName(),
                                  "dialog",
                                  tr("&Select Account"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        ga->setFactory(SAIL_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* SAIL_ActionSelectProject::factory() {
    RB_Action* a = new SAIL_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger(); by conditionalPlugin()

    const QString pluginType = "SAIL";
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
void SAIL_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("SAIL", 0, 9, 13)) {
            SAIL_DIALOGFACTORY->requestWarningDialog(tr("SAIL check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = SAIL_DIALOGFACTORY->getDialog(SAIL_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                SAIL_DIALOGFACTORY->requestWarningDialog(tr("Project not valid."));
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            SAIL_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = SAIL_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(SAIL_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        } else  {
            SAIL_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        SAIL_DIALOGFACTORY->deleteAllMdiWindows();
        SAIL_DIALOGFACTORY->deleteAllDockWidgets();

        if (SAIL_MODELFACTORY->getRootId() != "") {
            SAIL_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready,\n"
                                                       "select action from perspective menu."));
            SAIL_DIALOGFACTORY->commandMessage(tr("SAIL perspective is ready"));
            SAIL_DIALOGFACTORY->statusBarMessage(tr("SAIL perspective is ready"), 2000);
        }

    } else {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    SAIL_MODELFACTORY->emitState();
}
