/*****************************************************************
 * $Id: crm_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_actionselectproject.h"

#include "crm_dialogfactory.h"
#include "crm_projectdialog.h"
#include "db_actionfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


CRM_ActionSelectProject::CRM_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* CRM_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(CRM_ActionSelectProject::getName(),
                                  "dialog",
                                  tr("&Select Account"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        ga->setFactory(CRM_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* CRM_ActionSelectProject::factory() {
    RB_Action* a = new CRM_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger(); by conditionalPlugin()

    const QString pluginType = "CRM";
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
void CRM_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("CRM", 0, 9, 15)) {
            CRM_DIALOGFACTORY->requestWarningDialog(
                        tr("CRM check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                CRM_DIALOGFACTORY->requestWarningDialog(tr("Project not valid."));
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            CRM_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = CRM_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(CRM_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        } else  {
            CRM_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        CRM_DIALOGFACTORY->deleteAllMdiWindows();
        CRM_DIALOGFACTORY->deleteAllDockWidgets();

        if (CRM_MODELFACTORY->getRootId() != "") {
            CRM_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready,\n"
                                                       "select action from perspective menu."));
            CRM_DIALOGFACTORY->commandMessage(tr("CRM perspective is ready"));
            CRM_DIALOGFACTORY->statusBarMessage(tr("CRM perspective is ready"), 2000);
        }

    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    CRM_MODELFACTORY->emitState();
}
