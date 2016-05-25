/*****************************************************************
 * $Id: db_actionsystemdatabasedisconnect.cpp 1924 2013-04-11 16:59:22Z rutger $
 * Created: Dec 2, 2009 7:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemdatabasedisconnect.h"

#include "rb_database.h"
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mainwindow.h"


DB_ActionSystemDatabaseDisconnect::DB_ActionSystemDatabaseDisconnect()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemDatabaseDisconnect::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemDatabaseDisconnect::getName(),
                                  "dialog",
                                  tr("&Disconnect"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Disconnect from database ..."));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Disconnect from database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionSystemDatabaseDisconnect::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemDatabaseDisconnect::factory() {
    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Disconnect"),
                   tr("Disconnecting from the database\n"
                      "will close all database windows.\n"
                      "Do you want to continue?"));
    if (res != QMessageBox::Yes) return NULL;

    // Deactivate all actions that require a valid database ID
    DB_MODELFACTORY->setAllRootId("");
    // Close all MDI windows of all perspectives
    DB_DIALOGFACTORY->deleteAllMdiWindows(); // do not use closeAllMdiWindows here
    // Close all dockwidgets of all perspective
    DB_DIALOGFACTORY->deleteAllDockWidgets();
    // Disconnect of (default) database
    DB_MODELFACTORY->disconnectFromDatabase();

    DB_DIALOGFACTORY->getMainWindow()->setWindowTitle("Biluna");
    DB_DIALOGFACTORY->commandMessage("Database disconnected");
    DB_MODELFACTORY->emitState();

    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
//void DB_ActionSystemDatabaseDisconnect::trigger() {
//    //
//    // Prepare models
//    //
//    RB_MmObjectInterface* model = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTest);
//    model->setMainModel(true);
//    model->setRoot(); // equal to mModel->setRoot("");
//
//    // Fill model with some rows
//    model->insertRows(0, 1, QModelIndex());
//    model->insertRows(1, 1, QModelIndex());
//
//    // Child model and connect to child (table view),
////    RB_MmObjectInterface* childModel = DB_MmTestChild::getInstance();
//    RB_MmObjectInterface* childModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestChild);
//    childModel->setModel("DB_TestChildList", model);
//
//    // Activate models
//    mTableWidget->setModel();
//}
