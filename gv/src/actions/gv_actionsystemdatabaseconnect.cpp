/*****************************************************************
 * $Id: gv_actionsystemdatabaseconnect.cpp 2199 2014-12-08 21:41:10Z rutger $
 * Created: Apr 19, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsystemdatabaseconnect.h"

#include "gv_actionsystemselectproject.h"
#include "db_dialogfactory.h"
#include "rb_database.h"
#include "rb_mainwindow.h"
#include "rb_modelfactory.h"


GV_ActionSystemDatabaseConnect::GV_ActionSystemDatabaseConnect()
        : RB_Action() {
    // nothing
}


RB_GuiAction* GV_ActionSystemDatabaseConnect::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(GV_ActionSystemDatabaseConnect::getName(),
                                  "dialog",
                                  tr("&Connect"));
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
        ga->setFactory(GV_ActionSystemDatabaseConnect::factory);
    }
    return ga;
}

RB_Action* GV_ActionSystemDatabaseConnect::factory() {
    RB_ModelFactory* mf = NULL;

    if (!RB_Database::database().isOpen()) {
        RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogDbConnection);

        if (dlg->exec() == QDialog::Accepted) {
            mf = DB_DIALOGFACTORY->getModelFactory();
            QSqlDatabase defaultDb = RB_DATABASE->database();
            mf->setDatabase(defaultDb);
            DB_DIALOGFACTORY->getMainWindow()->setWindowTitle("Biluna - " + defaultDb.databaseName());
        } else {
            delete dlg;
            dlg = NULL;
            return NULL;
        }

        delete dlg;
    } else {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Database is already open."));
    }

    // Check whether table acc_project exists, before selecting a project
    QSqlDatabase db = mf->getDatabase();
    if (db.isOpen()) {
        if (!db.tables().contains("GV_Project", Qt::CaseInsensitive)) {
            return NULL;
        }
    }

    RB_Action* a = new GV_ActionSystemSelectProject();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}
