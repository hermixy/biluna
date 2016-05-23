/*****************************************************************
 * $Id: db_actiontestdialogmodel.cpp 1771 2012-09-06 21:13:20Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestdialogmodel.h"

#include "db_testdialogfactory.h"
#include "db_testmodelfactory.h"
#include "rb_mdiwindow.h"


DB_ActionTestDialogModel::DB_ActionTestDialogModel() : RB_Action() {

}


RB_GuiAction* DB_ActionTestDialogModel::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionTestDialogModel::getName(),
                                  "dialog",
                                  tr("&Dialog Model"));
//        action->setShortcut("Ctrl+W");
        ga->setStatusTip(tr("Test dialog model"));
        ga->setCommand("dia, dialog");
        ga->setKeycode("dia");
        ga->setStatusTip(tr("Test dialog model"));
        ga->setShortcut("Ctrl+D");
        ga->setToolTip("Test dialog model");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestDialogModel::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestDialogModel::factory() {
    RB_Action* a = new DB_ActionTestDialogModel();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestDialogModel::trigger() {
    // Check if parent MDI window is active
    if (!DB_TESTDIALOGFACTORY->isWidgetActive(DB_TestDialogFactory::WidgetTestTable)) {
        DB_TESTDIALOGFACTORY->requestWarningDialog(tr("Could not open dialog,\n"
                                                      "table Window not active."));
        return;
    }

//    // Prepare model(s)
//    RB_MmObjectInterface* parentModel = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTest);
//    QModelIndex parentIndex = parentModel->getCurrentIndex();
//    if (!parentIndex.isValid()) {
//        DB_TESTDIALOGFACTORY->requestWarningDialog(tr("No item selected, \n"
//                                                      "could not open dialog."));
//        return;
//    }
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTestDialog);
//    model->setRoot(parentIndex);

    // Create dialog
    RB_Dialog* dlg = DB_TESTDIALOGFACTORY->getDialog(DB_TestDialogFactory::WidgetTestDialog);
    dlg->exec();
    delete dlg;
}

