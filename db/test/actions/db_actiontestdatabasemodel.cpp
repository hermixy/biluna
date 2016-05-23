/*****************************************************************
 * $Id: db_actiontestdatabasemodel.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontestdatabasemodel.h"
#include "db_modelfactory.h"
#include "db_testdialogfactory.h"
#include "db_testmodelfactory.h"
#include "db_testobjectfactory.h"
#include "db_testtablewidget.h"
#include "rb_mdiwindow.h"


DB_ActionTestDatabaseModel::DB_ActionTestDatabaseModel()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionTestDatabaseModel::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionTestDatabaseModel::getName(),
                                  "fileclose",
                                  tr("&Database Model"));
        ga->setStatusTip(tr("Test database table model"));
        ga->setFactory(DB_ActionTestDatabaseModel::factory);
    }                                  
    return ga;
}

RB_Action* DB_ActionTestDatabaseModel::factory() {
    RB_Action* a = new DB_ActionTestDatabaseModel();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionTestDatabaseModel::trigger() {
    // No preliminary checks required

    // Prepare model(s)
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTest);
//    model->setRoot(DB_MODELFACTORY->getRootId());

    // Create widget
    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(
            DB_TestDialogFactory::WidgetTestTable);
//    DB_TestTableWidget* tw = dynamic_cast<DB_TestTableWidget*>(mdiWin->getWidget());
//    tw->init(DB_TestModelFactory::ModelTest, DB_TestModelFactory::ModelTestChild);
    mdiWin->show();
}
