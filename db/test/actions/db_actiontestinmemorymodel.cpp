/*****************************************************************
 * $Id: db_actiontestinmemorymodel.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestinmemorymodel.h"
#include "db_testdialogfactory.h"
#include "db_testmodelfactory.h"
#include "db_testobjectfactory.h"
#include "rb_mdiwindow.h"


DB_ActionTestInMemoryModel::DB_ActionTestInMemoryModel(/*DB_TestTableWidget* tw*/)
        : RB_Action() {
//    mTableWidget = tw;
}


RB_GuiAction* DB_ActionTestInMemoryModel::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionTestInMemoryModel::getName(),
                                  "fileclose",
                                  tr("&In-memory Model"));
        ga->setCommand("inmemory, inm");
        ga->setKeycode("im");
        ga->setStatusTip(tr("Test in-memory model"));
        ga->setShortcut("Ctrl+I");
        ga->setToolTip("Test object factory");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestInMemoryModel::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestInMemoryModel::factory() {
//    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
//            DB_TestDialogFactory::WidgetTestTable);
//    DB_TestTableWidget* tw = dynamic_cast<DB_TestTableWidget*>(mdiWin->getWidget());
//
//    if (mdiWin && tw) {
        RB_Action* a = new DB_ActionTestInMemoryModel(/*tw*/);
        // no graphicsView with eventhandler which deletes the action
        a->trigger();
        delete a;
        a = NULL;
//        // show window
//        mdiWin->show();
//    }
    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestInMemoryModel::trigger() {
    // Prepare model(s) if required
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTest);
//    model->setRoot(DB_TESTMODELFACTORY->getRoot()); // only for main models
//
//    // Activate models
//    mTableWidget->init(DB_TestModelFactory::ModelTest, DB_TestModelFactory::ModelTestChild);

    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
            DB_TestDialogFactory::WidgetTestTable);
    mdiWin->show();
}
