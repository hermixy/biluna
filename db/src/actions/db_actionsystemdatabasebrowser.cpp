/*****************************************************************
 * $Id: db_actionsystemdatabasebrowser.cpp 1644 2012-05-19 09:16:23Z rutger $
 * Created: Nov 26, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemdatabasebrowser.h"

#include "db_dialogfactory.h"



DB_ActionSystemDatabaseBrowser::DB_ActionSystemDatabaseBrowser()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionSystemDatabaseBrowser::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionSystemDatabaseBrowser::getName(),
                                  "dialog",
                                  tr("&Maintain/Export Database"));
//        ga->setStatusTip(tr("Connect to database"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
//        ga->setStatusTip(tr("Connect to database ..."));
//        ga->setShortcut("Ctrl+B");
//        ga->setToolTip("Connect to database");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionSystemDatabaseBrowser::factory);
    }
    return ga;
}

RB_Action* DB_ActionSystemDatabaseBrowser::factory() {
    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogDbBrowser);

    // Modal
    // dlg->exec();

    // Modeless, delete on close or by deletion of mainwindow (is parent widget)
    dlg->show();
    dlg->raise();
    dlg->activateWindow();

    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
//void DB_ActionSystemDatabaseBrowser::trigger() {
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
