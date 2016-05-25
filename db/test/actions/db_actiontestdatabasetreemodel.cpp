/*****************************************************************
 * $Id: db_actiontestdatabasetreemodel.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestdatabasetreemodel.h"
#include "db_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "db_testtreewidget.h"
#include "rb_mdiwindow.h"


DB_ActionTestDatabaseTreeModel::DB_ActionTestDatabaseTreeModel()
                                : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestDatabaseTreeModel::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga == NULL) {
        ga = new RB_GuiAction(DB_ActionTestDatabaseTreeModel::getName(),
                                  "fileclose",
                                  tr("Database T&ree Model"));
        ga->setCommand("dtr, dbtr");
//        ga->setKeycode("itr");
        ga->setStatusTip(tr("Test in-memory tree model"));
//        ga->setShortcut("Ctrl+R");
//        ga->setToolTip("Test object factory");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestDatabaseTreeModel::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestDatabaseTreeModel::factory() {
    RB_Action* a = new DB_ActionTestDatabaseTreeModel();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestDatabaseTreeModel::trigger() {
    // No preliminary checks rquired

    // Prepare model(s)
//    RB_MmObjectInterface* model = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestTree);
//    model->setRoot(DB_MODELFACTORY->getRootId());

    // Create widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(DB_DialogFactory::WidgetTestTree);
//    DB_TestTreeWidget* tw = dynamic_cast<DB_TestTreeWidget*>(mdiWin->getWidget());
//    tw->init(DB_ModelFactory::ModelTestTree, DB_ModelFactory::ModelTestTreeChild);
    mdiWin->show();
}
