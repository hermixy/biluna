/*****************************************************************
 * $Id: sail_actionselectproject.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_actionselectproject.h"

#include "mrp_dialogfactory.h"
#include "mrp_selectprojectdialog.h"
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


MRP_ActionSelectProject::MRP_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* MRP_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(MRP_ActionSelectProject::getName(),
                                  "dialog",
                                  tr("&Select Account"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        ga->setFactory(MRP_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* MRP_ActionSelectProject::factory() {
    RB_Action* a = new MRP_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger(); by conditionalPlugin()

    const QString pluginType = "MRP";
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
void MRP_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("MRP", 0, 9, 15)) {
            MRP_DIALOGFACTORY->requestWarningDialog(tr("MRP check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = MRP_DIALOGFACTORY->getDialog(MRP_DialogFactory::DialogSelectProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                MRP_DIALOGFACTORY->requestWarningDialog(tr("Project not valid."));
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            MRP_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = MRP_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(MRP_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        } else  {
            MRP_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        MRP_DIALOGFACTORY->deleteAllMdiWindows();
        MRP_DIALOGFACTORY->deleteAllDockWidgets();

        if (MRP_MODELFACTORY->getRootId() != "") {
            MRP_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready,\n"
                                                       "select action from perspective menu."));
            MRP_DIALOGFACTORY->commandMessage(tr("MRP perspective is ready"));
            MRP_DIALOGFACTORY->statusBarMessage(tr("MRP perspective is ready"), 2000);
        }

    } else {
        MRP_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    MRP_MODELFACTORY->emitState();
}
