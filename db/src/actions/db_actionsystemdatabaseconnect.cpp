/*****************************************************************
 * $Id: db_actionsystemdatabaseconnect.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemdatabaseconnect.h"

#include "db_actionsystemselectproject.h"
#include "db_connectiondialog.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"
#include "rb_database.h"
#include "rb_mainwindow.h"
#include "rb_modelfactory.h"


DB_ActionSystemDatabaseConnect::DB_ActionSystemDatabaseConnect()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemDatabaseConnect::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemDatabaseConnect::getName(),
                                  "dialog",
                                  tr("C&onnect"));
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
        ga->setFactory(DB_ActionSystemDatabaseConnect::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemDatabaseConnect::factory() {
    RB_ModelFactory* mf = NULL;

    if (!RB_DATABASE->database().isOpen()) {
        RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(
                    DB_DialogFactory::DialogDbConnection);

        if (dlg->exec() == QDialog::Accepted) {
            mf = DB_DIALOGFACTORY->getModelFactory();
            mf->setDatabase(RB_DATABASE->database());
            DB_DIALOGFACTORY->getMainWindow()->setWindowTitle(
                        "Biluna - " + RB_DATABASE->database().databaseName());
        } else {
            dlg->deleteLater();
            return NULL;
        }

        DB_ConnectionDialog* conDlg = dynamic_cast<DB_ConnectionDialog*>(dlg);
        DB_PERMISSIONHANDLER->setUserName(conDlg->userName());
        DB_PERMISSIONHANDLER->setPassword(conDlg->userPassword());
        dlg->deleteLater();
    } else {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Database is already open."));
    }

    if (!mf) {
        RB_DEBUG->error("DB_ActionSystemDatabaseConnect::factory() "
                        "mf is NULL ERROR");
        QString connectionName = RB_DATABASE->database().connectionName();
        RB_DATABASE->database().close();
        RB_DATABASE->removeDatabase(connectionName);
        return nullptr;
    }

    // Check whether tables exist (from an existing database)
    QSqlDatabase defaultDb = RB_DATABASE->database();
    if (defaultDb.isOpen()) {
        if (defaultDb.tables().size() > 0
            && !defaultDb.tables().contains("DB_Project", Qt::CaseInsensitive)) {
            int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Existing database"),
                           tr("Tables exist in database,\n"
                              "Do you want to create the Biluna DB tables?"));
            if (res != QMessageBox::Yes) {
                // Existing database of other application and the user
                // does not want to create the DB tables
                DB_MODELFACTORY->emitState();
                return NULL;
            }
        }
    } else {
        DB_DIALOGFACTORY->commandMessage("Not connected to database");
        DB_DIALOGFACTORY->statusBarMessage(tr("Database connection invalid"), 2000);
        return nullptr;
    }

    DB_DIALOGFACTORY->commandMessage(tr("Database connected"));
    DB_DIALOGFACTORY->statusBarMessage(tr("Database connection is ready"), 2000);

    // Check whether DB tables exists, if not run create tables in database
    if (!DB_MODELFACTORY->createMissingTables("DB", 0, 9, 14)) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("DB check- and update database ERROR."));
        DB_MODELFACTORY->emitState();
        return nullptr;
    }

    DB_PERMISSIONHANDLER->refresh(); // with username and password

    if (!DB_PERMISSIONHANDLER->isValidDbUser()) {
        QString connectionName = RB_DATABASE->database().connectionName();
        RB_DATABASE->database().close();
        RB_DATABASE->removeDatabase(connectionName);
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Not a valid user,\n"
                       "database connection closed."));
        DB_DIALOGFACTORY->commandMessage(tr("Database disconnected"));
        return nullptr;
    }

    // Select project and if new database create the tables
    RB_Action* a = new DB_ActionSystemSelectProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;

    if (DB_MODELFACTORY->getRootId() != "") {
        DB_DIALOGFACTORY->requestInformationDialog(
                    tr("Database connection is ready,\n"
                       "select a perspective from the menu."));
    }

    a = NULL;
    return a;
}
