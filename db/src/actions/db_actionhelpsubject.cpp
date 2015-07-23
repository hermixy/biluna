/*****************************************************************
 * $Id: db_actionhelpsubject.cpp 1594 2012-03-29 13:38:43Z rutger $
 * Created: Nov 26, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionhelpsubject.h"

#include "db_dialogfactory.h"
#include "rb_mdiwindow.h"


DB_ActionHelpSubject::DB_ActionHelpSubject() : RB_Action() {

}

RB_GuiAction* DB_ActionHelpSubject::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionHelpSubject::getName(),
                              tr("&Subject"));
        ga->setStatusTip(tr("Help subject"));
        ga->setShortcut(QKeySequence::HelpContents); // same as "F1"
        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionHelpSubject::factory);
    }
    return ga;
}

RB_Action* DB_ActionHelpSubject::factory() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    RB_MdiWindow* mdiW = mw->getActiveMdiWindow();

    if (mdiW) {
        RB_String str = mdiW->getWidget()->getHelpSubject().toLower();
        mw->slotHelpSubject(str);
    } else {
        mw->slotHelpSubject("index");
    }
    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
//void DB_ActionSettingsDatabaseDisconnect::trigger() {
//    //
//    // Prepare models
//    //
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTest);
//    model->setMainModel(true);
//    model->setRoot(); // equal to mModel->setRoot("");
//
//    // Fill model with some rows
//    model->insertRows(0, 1, QModelIndex());
//    model->insertRows(1, 1, QModelIndex());
//
//    // Child model and connect to child (table view),
////    RB_MmObjectInterface* childModel = DB_MmTestChild::getInstance();
//    RB_MmObjectInterface* childModel = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTestChild);
//    childModel->setModel("DB_TestChildList", model);
//
//    // Activate models
//    mTableWidget->setModel();
//}
