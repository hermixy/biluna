/*****************************************************************
 * $Id: srm_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionselectproject.h"

#include "srm_dialogfactory.h"
#include "srm_projectdialog.h"
#include "db_modelfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


SRM_ActionSelectProject::SRM_ActionSelectProject()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionSelectProject::getName(),
                                  "dialog",
                                  tr("&Select Account"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        ga->setFactory(SRM_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* SRM_ActionSelectProject::factory() {
    RB_Action* a = new SRM_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("SRM", 0, 9, 13)) {
            SRM_DIALOGFACTORY->requestWarningDialog(tr("SRM check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                SRM_DIALOGFACTORY->requestWarningDialog(tr("Project not valid."));
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            SRM_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = SRM_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(SRM_MODELFACTORY->getDatabase(), RB2::ResolveNone);
        } else  {
            SRM_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        SRM_DIALOGFACTORY->deleteAllMdiWindows();
        SRM_DIALOGFACTORY->deleteAllDockWidgets();

        if (SRM_MODELFACTORY->getRootId() != "") {
            SRM_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready,\n"
                                                       "select action from perspective menu."));
            SRM_DIALOGFACTORY->commandMessage(tr("SRM perspective is ready"));
            SRM_DIALOGFACTORY->statusBarMessage(tr("SRM perspective is ready"), 2000);
        }

    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    SRM_MODELFACTORY->emitState();
}
