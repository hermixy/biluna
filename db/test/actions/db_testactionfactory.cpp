/*****************************************************************
 * $Id: db_testactionfactory.cpp 1419 2011-05-16 13:43:55Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testactionfactory.h"

#include "db_actionfactory.h"
#include "db_actionsystemdatabasebrowser.h"
#include "db_actionsystemdatabaseconnect.h"
#include "db_actionsystemdatabasedisconnect.h"
#include "db_actionsystemdatabasemapping.h"
#include "db_actiontest.h"
#include "db_actiontestobject.h"
#include "db_actiontestcleartext.h"
#include "db_actiontestdatabasetreemodel.h"
#include "db_actiontestdatabasemodel.h"
#include "db_actiontestdbobject.h"
#include "db_actiontestdbtabletree.h"
#include "db_actiontestdialogmodel.h"
#include "db_actiontestfactory.h"
#include "db_actiontestinmemorymodel.h"
#include "db_actiontestinmemorytreemodel.h"
#include "db_actiontestobject.h"
#include "db_actiontestrelation.h"
#include "db_actiontestsystemselectproject.h"
#include "db_testdialogfactory.h"


DB_TestActionFactory* DB_TestActionFactory::mActiveFactory = 0;


/**
 * Constructor, register with DB_ActionFactory
 */
DB_TestActionFactory::DB_TestActionFactory(RB_MainWindow* mw)
        : RB_ActionFactory(mw) {
    RB_DEBUG->print("DB_TestActionFactory::DB_TestActionFactory()");
    DB_ACTIONFACTORY->registerFactory(this);
}

/**
 * Destructor
 */
DB_TestActionFactory::~DB_TestActionFactory() {
    DB_ACTIONFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
}

/**
 * @return unique instance of this action factory
 */
DB_TestActionFactory* DB_TestActionFactory::getInstance(RB_MainWindow* mw) {
    if (!mActiveFactory) {
        mActiveFactory = new DB_TestActionFactory(mw);
    }
    return mActiveFactory;
}

/**
 * Get and add the 'Test' actions to the menu and toolbar
 */
void DB_TestActionFactory::getTestMenu(QMenu* menu, QToolBar* tb = NULL) {
    RB_GuiAction* ga = NULL;
    // QMenu* subMenu = NULL;

    ga = DB_ActionTestSystemSelectProject::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestObject::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestFactory::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestRelation::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestInMemoryModel::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestDatabaseModel::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    menu->addSeparator();

    ga = DB_ActionTestInMemoryTreeModel::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestDatabaseTreeModel::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestDbTableTree::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    menu->addSeparator();

    ga = DB_ActionTestDbObject::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);

    ga = DB_ActionTestClearText::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_TestDialogFactory::WidgetTestText);
    connect(mMainWindow, SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));

    menu->addSeparator();

    ga = DB_ActionTestDialogModel::createGuiAction();
    menu->addAction(ga);
    tb->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(DB_TestDialogFactory::WidgetTestTable);
    ga->addEnabledType(DB_TestDialogFactory::WidgetTestTree);
    connect(mMainWindow, SIGNAL(subWindowChanged(int)),
            ga, SLOT(slotSetEnabled(int)));
}

/**
 * Get Settings menu items, for:
 * - Database connection
 * - Administrator, with submenu items
 * - - Browse database
 * - - Map data to database
 * @param menu
 * @param tb toolbar, not used here
 */
void DB_TestActionFactory::getSystemMenu(QMenu* menu, QToolBar* /* tb */) {
    RB_GuiAction* ga = NULL;
    QMenu* subMenu = NULL;

    ga = DB_ActionSystemDatabaseConnect::createGuiAction();
    menu->addAction(ga);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsNotSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemDatabaseDisconnect::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionTestSystemSelectProject::createGuiAction();
    menu->addAction(ga);
    ga->setEnabled(false);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    // create and add submenu
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&Administrator"));
    menu->addMenu(subMenu);

    ga = DB_ActionSystemDatabaseBrowser::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));

    ga = DB_ActionSystemDatabaseMapping::createGuiAction();
    subMenu->addAction(ga);
    ga->setEnabled(false);
    ga->addEnabledType(RB2::ValidTrue);
    connect(DB_MODELFACTORY, SIGNAL(databaseIsSet(int)),
            ga, SLOT(slotSetEnabled(int)));
}

