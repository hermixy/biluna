/*****************************************************************
 * $Id: db_testmainwindow.cpp 1958 2013-08-05 09:25:33Z rutger $
 * Created: Nov 5, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testmainwindow.h"

#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"
#include "db_testactionfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "rb_dockwidget.h"
#include "rb_mdiwindow.h"

/**
 * Constructor
 */
DB_TestMainWindow::DB_TestMainWindow(QWidget* parent, Qt::WindowFlags flags)
            : RB_MainWindow(parent, flags) {
    RB_DEBUG->print("DB_TestMainWindow::DB_TestMainWindow()");

    // In RB_MainWindow
    // DB_ModelFactory::getInstance(this);
    // DB_DialogFactory::getInstance(this);
    // DB_ActionFactory::getInstance(this);
    // DB_UtilityFactory::getInstance();

    DB_TestActionFactory::getInstance(this);
    initActions();
    initViews();

    setWindowTitle("DB Test");
}

/**
 * Destructor
 */
DB_TestMainWindow::~DB_TestMainWindow() {
    // Delete of DB_TestActionFactory not required
    // because factory is registered with DB_ActionFactory
    RB_DEBUG->print("DB_TestMainWindow::~DB_TestMainWindow() OK");
}

/**
 * File new slot, clears current in-memory model
 */
void DB_TestMainWindow::slotFileNew() {
    // current model data to be saved?
    if (!maybeSave()) {
        return;
    }

    // Close all
    mMdiArea->closeAllSubWindows();
    QList<QMdiSubWindow*> list = mMdiArea->subWindowList();
    list.clear();
    DB_MODELFACTORY->setRoot();
    // Get subwindow, for example from db_actiontestinmemorymodel.h
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(DB_DialogFactory::WidgetTestTable);
    mdiWin->show();
}

/**
 * File open in current in-memory model
 */
bool DB_TestMainWindow::slotFileOpen() {
    RB_DEBUG->print("RB_MainWindow::slotFileOpen()");
    bool success = RB_MainWindow::slotFileOpen();
    // open relevant window
    return success;
}

/**
 * Print open MDI window
 */
void DB_TestMainWindow::slotFilePrint() {

}

/**
 * Initializes all RB_GuiActions of the application. Actions are require here
 * a connection to a slot if the action does not have a 'factory()' function,
 * refer to DB_TestActionObject for explanation.
 */
void DB_TestMainWindow::initActions() {
    RB_DEBUG->print("ApplicationWindow::initActions()");

    QMenu* menu = NULL;
    QMenu* subMenu = NULL;
    QToolBar* tb = NULL;

    // File actions:
    //
    menu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    menu->setTitle(tr("&File"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(DB_DIALOGFACTORY->getMainWindow());
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_file");
    tb->setWindowTitle(tr("File"));
    addToolBar(Qt::TopToolBarArea, tb);

    // File > New actions:
    //
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&New"));
    menu->addMenu(subMenu);
    DB_ACTIONFACTORY->getFileNewMenu(subMenu, tb);
    DB_ACTIONFACTORY->getFileMenu(menu, tb);


    // Edit actions:
    //
    menu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    menu->setTitle(tr("&Edit"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(DB_DIALOGFACTORY->getMainWindow());
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_edit");
    tb->setWindowTitle(tr("Edit"));
    addToolBar(Qt::TopToolBarArea, tb);
    DB_ACTIONFACTORY->getEditMenu(menu, tb);

    // Edit > HTML actions:
    //
//    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
//    subMenu->setTitle(tr("&HTML"));
//    menu->addMenu(subMenu);
//    tb = new QToolBar(this);
//    tb->setFixedHeight(30); // (32) ?
//    tb->setIconSize(QSize(16,16));
//    tb->setObjectName("toolbar_edit_html");
//    tb->setWindowTitle(tr("HTML"));
//    addToolBar(Qt::TopToolBarArea, tb);
//    DB_ACTIONFACTORY->getEditHtmlMenu(subMenu, tb);

    // Format actions:
    //
    menu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    menu->setTitle(tr("For&mat"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(DB_DIALOGFACTORY->getMainWindow());
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_format");
    tb->setWindowTitle(tr("Format"));
    addToolBar(Qt::TopToolBarArea, tb);
    DB_ACTIONFACTORY->getFormatMenu(menu, tb);

    // Test actions:
    //
    menu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    menu->setTitle(tr("&Test"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(DB_DIALOGFACTORY->getMainWindow());
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_test");
    tb->setWindowTitle(tr("Test"));
    addToolBar(Qt::TopToolBarArea, tb);
    DB_TESTACTIONFACTORY->getTestMenu(menu, tb);

    // Window actions:
    //
    initWindowActions();

    // System actions:
    //
    menu = new QMenu(tr("S&ystem"), DB_DIALOGFACTORY->getMainWindow());
    menuBar()->addMenu(menu);
    DB_ACTIONFACTORY->getSystemMenu(menu, NULL);

    // Help actions:
    //
    menu = new QMenu(tr("&Help"), DB_DIALOGFACTORY->getMainWindow());
    menuBar()->addMenu(menu);
    DB_ACTIONFACTORY->getHelpMenu(menu, NULL);



    // ...

    // Debug actions, for debugging only
    //
//    initDebugActions();

}

void DB_TestMainWindow::initViews() {
    RB_DockWidget* dw = NULL;

    // ...

    RB_DEBUG->print("  command widget..");

    dw = DB_DIALOGFACTORY->getDockWidget(DB_DialogFactory::WidgetCommand,
                                         Qt::BottomDockWidgetArea);
    dw->setMinimumHeight(30);
    dw->hide();
}
