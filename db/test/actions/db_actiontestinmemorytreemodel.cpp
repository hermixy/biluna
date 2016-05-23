/*****************************************************************
 * $Id: db_actiontestinmemorytreemodel.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestinmemorytreemodel.h"
#include "db_testdialogfactory.h"
#include "db_testmodelfactory.h"
#include "db_testobjectfactory.h"
#include "rb_mdiwindow.h"


DB_ActionTestInMemoryTreeModel::DB_ActionTestInMemoryTreeModel(/*DB_TestTreeWidget* tw*/)
                                : RB_Action() {
//    mTreeWidget = tw;
}


RB_GuiAction* DB_ActionTestInMemoryTreeModel::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga == NULL) {
        ga = new RB_GuiAction(DB_ActionTestInMemoryTreeModel::getName(),
                                  "fileclose",
                                  tr("In-memory T&ree Model"));
        ga->setCommand("inmemorytree, int");
        ga->setKeycode("itr");
        ga->setStatusTip(tr("Test in-memory tree model"));
        ga->setShortcut("Ctrl+R");
        ga->setToolTip("Test object factory");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestInMemoryTreeModel::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestInMemoryTreeModel::factory() {
//    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
//            DB_TestDialogFactory::WidgetTestTree);
//    DB_TestTreeWidget* tw = dynamic_cast<DB_TestTreeWidget*>(mdiWin->getWidget());
//
//    if (mdiWin && tw) {
        RB_Action* a = new DB_ActionTestInMemoryTreeModel(/*tw*/);
        // no graphicsView with eventhandler which deletes the action
        a->trigger();
        delete a;
        a = NULL;
        // show window
//        mdiWin->show();
//    }
    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestInMemoryTreeModel::trigger() {
    // Prepare model(s) if required
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTestTree);
//    model->setRoot(DB_TESTMODELFACTORY->getRoot()); // only for main models

    // fill model with some rows
//    model->insertRows(0, 1, QModelIndex());
//    model->insertRows(1, 1, QModelIndex());
//    QModelIndex parent = model->index(0, 0, QModelIndex());
//    model->insertRows(0, 1, parent);
//    model->insertRows(1, 1, parent);
//    model->insertRows(2, 1, parent);
//    QModelIndex child = model->index(0, 0, parent);
//    model->insertRows(0, 1, child);

    // Set and activate models
//    mTreeWidget->init(DB_TestModelFactory::ModelTestTree, DB_TestModelFactory::ModelTestTreeChild);

    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
            DB_TestDialogFactory::WidgetTestTree);
    mdiWin->show();
}
