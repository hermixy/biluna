/*****************************************************************
 * $Id: gv_mainwindow.cpp 1148 2010-05-08 11:53:42Z rutger $
 * Created: Mar 19, 2008 16:34:42 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_mainwindow.h"

#include "gv_actionfactory.h"
#include "gv_dialogfactory.h"
#include "gv_modelfactory.h"
#include "gv_objectfactory.h"

#include "db_actionfactory.h"
#include "db_dialogfactory.h"

#include "rb_mdiwindow.h"

/**
 * Constructor
 */
GV_MainWindow::GV_MainWindow(QWidget* parent, Qt::WindowFlags flags)
            : RB_MainWindow(parent, flags) {
    RB_DEBUG->print("GV_MainWindow::GV_MainWindow()");

//    GV_ObjectFactory::getInstance();
    GV_ModelFactory::getInstance(this, RB_Database());
    GV_DialogFactory::getInstance(this);
    GV_ActionFactory::getInstance(this);

    initActions();
    initViews();

    mEdition = " [Professional]";
    mModules << "DB" << "GV"; // TODO: generate on basis of the available dialogfactories or plugins
    setWindowTitle("GV");

}

/**
 * Destructor
 */
GV_MainWindow::~GV_MainWindow() {
    // nothing, factories deleted in ~RB_MainWindow()
    RB_DEBUG->print("GV_MainWindow::~GV_MainWindow() OK");
}

/**
 * File new slot, clears current in-memory model
 */
void GV_MainWindow::slotFileNew() {
    // current model data to be saved?
    if (!maybeSave()) {
        return;
    }

    // Close all
//    mMdiArea->closeAllSubWindows();
//    QList<QMdiSubWindow*> list = mMdiArea->subWindowList();
//    list.clear();
//    mModelFactory->setRoot();
//    // Get subwindow, for example from db_actiontestinmemorymodel.h
//    RB_MdiWindow* mdiWin = DB_TESTDIALOGFACTORY->getMdiWindow(DB_TestDialogFactory::WidgetTestTable);
//    RB_Widget* tw = mdiWin->getWidget();
//    // Prepare and activate model(s) if required
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTest);
//    model->setRoot(DB_TESTMODELFACTORY->getRoot()); // only for main models
//    tw->init(DB_TestModelFactory::ModelTest, DB_TestModelFactory::ModelTestChild);
//    // Show subwindow
//    mdiWin->show();
}

/**
 * File open in current in-memory model
 */
bool GV_MainWindow::slotFileOpen() {
    RB_DEBUG->print("RB_MainWindow::slotFileOpen()");
    bool success = RB_MainWindow::slotFileOpen();
    // open relevant window
    return success;
}

/**
 * Print open MDI window
 */
void GV_MainWindow::slotFilePrint() {

}

/**
 * Initializes all RB_GuiActions of the application. Actions are require here
 * a connection to a slot if the action does not have a 'factory()' function,
 * refer to DB_TestActionObject for explanation.
 */
void GV_MainWindow::initActions() {
    RB_DEBUG->print("ApplicationWindow::initActions()");

    QMenu* menu = NULL;
    QToolBar* tb = NULL;
    QMenu* subMenu = NULL;


    // File actions:
    //
    menu = new QMenu(this);
    menu->setTitle(tr("&File"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(this);
    tb->setFixedHeight(30); // (32) ?
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_file");
    tb->setWindowTitle(tr("File"));
    addToolBar(Qt::TopToolBarArea, tb);

    // File > New actions:
    //
    subMenu = new QMenu(DB_DIALOGFACTORY->getMainWindow());
    subMenu->setTitle(tr("&New"));
    menu->addMenu(subMenu);
    // DB_ACTIONFACTORY->getFileNewMenu(subMenu, NULL);
    GV_ACTIONFACTORY->getFileNewMenu(subMenu, tb);
    DB_ACTIONFACTORY->getFileMenu(menu, tb);

    // Test actions:
    //
//    menu = new QMenu(this);
//    menu->setTitle(tr("T&est"));
//    menuBar()->addMenu(menu);
//    tb = new QToolBar(this);
//    tb->setFixedHeight(30); // (32) ?
//    tb->setIconSize(QSize(16,16));
//    tb->setObjectName("toolbar_test");
//    tb->setWindowTitle(tr("Test"));
//    addToolBar(Qt::TopToolBarArea, tb);
//    DB_TESTACTIONFACTORY->getTestMenu(menu, tb);

    // GV basic drawing actions:
    //
    menu = new QMenu(this);
    menu->setTitle(tr("&Drawing"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(this);
    tb->setFixedHeight(30); // (32) ?
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_accounting");
    tb->setWindowTitle(tr("Accounting"));
    addToolBar(Qt::TopToolBarArea, tb);
    GV_ACTIONFACTORY->getDrawingMenu(menu, tb);

    // Window actions:
    //
    this->initWindowActions();

    // Settings actions:
    //
    menu = new QMenu(tr("S&ystem"), this);
    menuBar()->addMenu(menu);
    GV_ACTIONFACTORY->getSystemMenu(menu, NULL);

    // Help actions:
    //
    menu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(menu);
    DB_ACTIONFACTORY->getHelpMenu(menu, NULL);



    // ...

    // Debug actions, for debugging only
    //
//    this->initDebugActions();
}

void GV_MainWindow::initViews() {
    QDockWidget* dw = NULL;

    // ...

    RB_DEBUG->print("  command widget..");
    dw = new QDockWidget(this);
    RB_Widget* wgt = DB_DIALOGFACTORY->newWidget(DB_DialogFactory::WidgetCommand, dw);
//    dw->setAttribute(Qt::WA_DeleteOnClose);
    dw->setWidget(wgt);
//    connect(this, SIGNAL(windowsChanged(bool)), wgt, SLOT(setEnabled(bool)));
    dw->setMinimumHeight(30);
    dw->setWindowTitle(wgt->getName());
    dw->setObjectName("dockwidget_commandline");
    addDockWidget(Qt::BottomDockWidgetArea, dw);
    dw->hide();
}

//void GV_MainWindow::changeEvent(QEvent *e)
//{
//    QMainWindow::changeEvent(e);
//    switch (e->type()) {
//    case QEvent::LanguageChange:
//        retranslateUi(this);
//        break;
//    default:
//        break;
//    }
//}

