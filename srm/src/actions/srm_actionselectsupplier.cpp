/*****************************************************************
 * $Id: srm_actionselectsupplier.cpp 1893 2013-02-21 21:41:33Z rutger $
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
#include "db_modelfactory.h"
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
                                  tr("&Select Company"));
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
        ga->setFactory(CRM_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* CRM_ActionSelectProject::factory() {
    RB_Action* a = new CRM_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void CRM_ActionSelectProject::trigger() {
    RB_Database dbConnections;

    if (dbConnections.database().isOpen()) {
        // Check whether DB tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->addMissingTables("ACC")) {
            CRM_DIALOGFACTORY->requestWarningDialog(tr("ACC check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();
            if (!obj) {
                delete dlg;
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
        delete dlg;
        CRM_DIALOGFACTORY->deleteAllMdiWindows();
        CRM_DIALOGFACTORY->deleteAllDockWidgets();

        if (CRM_MODELFACTORY->getRootId() != "") {
            CRM_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready,\n"
                                                       "select action from perspective menu."));
            CRM_DIALOGFACTORY->commandMessage(tr("ACC perspective is ready"));
            CRM_DIALOGFACTORY->statusBarMessage(tr("ACC perspective is ready"), 2000);
        }

    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }
}
