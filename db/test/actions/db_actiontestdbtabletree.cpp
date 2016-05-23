/*****************************************************************
 * $Id: db_actiontestdbtabletree.cpp 1419 2011-05-16 13:43:55Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestdbtabletree.h"
#include "db_modelfactory.h"
#include "db_testdialogfactory.h"
#include "db_testmodelfactory.h"
#include "db_testobjectfactory.h"
#include "db_testtreewidget.h"
#include "rb_mdiwindow.h"


DB_ActionTestDbTableTree::DB_ActionTestDbTableTree()
                                : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestDbTableTree::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga == NULL) {
        ga = new RB_GuiAction(DB_ActionTestDbTableTree::getName(),
                                  "fileclose",
                                  tr("Database Table Tree Model"));
        ga->setCommand("dttr, dbttr");
//        ga->setKeycode("itr");
        ga->setStatusTip(tr("Test table tree model"));
//        ga->setShortcut("Ctrl+R");
//        ga->setToolTip("Test object factory");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionTestDbTableTree::factory);
    }
    return ga;
}

RB_Action* DB_ActionTestDbTableTree::factory() {
    RB_Action* a = new DB_ActionTestDbTableTree();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestDbTableTree::trigger() {
    // No preliminary checks rquired

    // Create widget
    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(DB_TestDialogFactory::WidgetTestTableTree);
    mdiWin->show();
}
