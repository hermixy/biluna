/*****************************************************************
 * $Id: acc_actionselectproject.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Dec 21, 2009 21:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionselectproject.h"

#include "acc_dialogfactory.h"
#include "acc_projectdialog.h"
#include "acc_utilityfactory.h"
#include "db_actionfactory.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"
#include "rb_database.h"
#include "rb_mainwindow.h"


ACC_ActionSelectProject::ACC_ActionSelectProject()
        : RB_Action() {
    // nothing
}

ACC_ActionSelectProject::~ACC_ActionSelectProject() {
    // nothing
}


RB_GuiAction* ACC_ActionSelectProject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(ACC_ActionSelectProject::getName(),
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
        ga->setFactory(ACC_ActionSelectProject::factory);
    }
    return ga;
}

RB_Action* ACC_ActionSelectProject::factory() {
    RB_Action* a = new ACC_ActionSelectProject();
    // no graphicsView with eventhandler which deletes the action
    // a->trigger();
    DB_PERMISSIONHANDLER->conditionalPlugin(a, "ACC");
    delete a;
    a = nullptr;
    DB_ACTIONFACTORY->closePlugin("ACC");
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSelectProject::trigger() {
    if (RB_DATABASE->database().isOpen()) {
        // Check whether ACC tables exists, if not run create tables in database
        if (!DB_MODELFACTORY->createMissingTables("ACC", 0, 9, 14)) {
            ACC_DIALOGFACTORY->requestWarningDialog(tr("ACC check- and update database ERROR."));
            return;
        }

        // Open project selection dialog
        RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogProject);

        if (dlg->exec() == QDialog::Accepted) {
            RB_ObjectBase* obj = dlg->getCurrentObject();

            if (!obj) {
                dlg->deleteLater();
                ACC_DIALOGFACTORY->requestWarningDialog(tr("Project not valid."));
                return;
            }

            RB_String id = obj->getId();
            // Set root id of all models
            ACC_MODELFACTORY->setRootId(id);
            // Get the main project root object for this application
            RB_ObjectBase* root = ACC_MODELFACTORY->getRoot();
            // Read the object from the database for further use
            root->dbRead(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            // Refresh all utility factories
            ACC_UTILITYFACTORY->refresh();
        } else  {
            ACC_MODELFACTORY->setRootId("");
        }

        dlg->deleteLater();
        ACC_DIALOGFACTORY->deleteAllMdiWindows();
        ACC_DIALOGFACTORY->deleteAllDockWidgets();

        if (ACC_MODELFACTORY->getRootId() != "") {
            ACC_DIALOGFACTORY->requestWarningDialog(tr("Perspective is ready,\n"
                                                       "select action from perspective menu."));
            ACC_DIALOGFACTORY->commandMessage(tr("ACC perspective is ready"));
            ACC_DIALOGFACTORY->statusBarMessage(tr("ACC perspective is ready"), 2000);
        }

    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Not connected to database."));
    }

    ACC_MODELFACTORY->emitState();
}
