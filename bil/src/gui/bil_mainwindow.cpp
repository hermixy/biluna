/*****************************************************************
 * $Id: bil_mainwindow.cpp 2195 2014-11-10 12:26:20Z rutger $
 * Created: Dec 16, 2010 16:34:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna BIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "bil_mainwindow.h"

#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_dashboardwidget.h"

#include "rb_dockwidget.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"


/**
 * Constructor
 */
BIL_MainWindow::BIL_MainWindow(QWidget* parent, Qt::WindowFlags flags)
            : RB_MainWindow(parent, flags) {
    RB_DEBUG->print("BIL_MainWindow::BIL_MainWindow()");

    initActions();
    initViews();

#ifdef debug
    initDebug();
#endif

    mEdition = "[Professional]";
    // TODO: generate on basis of the available dialogfactories or plugins
    mModules << "BIL (DB)" << "ACC" << "CRM" << "SRM";

#ifdef BILUNA_DEMO
    setWindowTitle("Biluna [Demo]");
    // Close the application in case of a demo version, now in main.cpp
    // QTimer::singleShot(600000, this, SLOT(close())); refer also QTimer documentation.
#else
    // changed when connecting to database db_actionsystemdatabaseconnect.cpp
    setWindowTitle("Biluna");
#endif

    // After 100 milliseconds open dashboard
    QTimer::singleShot(100, this, SLOT(slotDashboard()));
}

/**
 * Destructor
 */
BIL_MainWindow::~BIL_MainWindow() {
    // nothing, factories deleted in ~RB_MainWindow()
    RB_DEBUG->print("BIL_MainWindow::~BIL_MainWindow() OK");
}

/**
 * File new slot, clears current in-memory model
 */
void BIL_MainWindow::slotFileNew() {
    // current model data to be saved?
    if (!maybeSave()) {
        return;
    }
}

/**
 * File open in current in-memory model
 */
bool BIL_MainWindow::slotFileOpen() {
    RB_DEBUG->print("RB_MainWindow::slotFileOpen()");
    bool success = RB_MainWindow::slotFileOpen();
    // open relevant window
    return success;
}

/**
 * Initializes all RB_GuiActions of the application. Actions are require here
 * a connection to a slot if the action does not have a 'factory()' function,
 * refer to DB_TestActionObject for explanation.
 */
void BIL_MainWindow::initActions() {
    RB_DEBUG->print("ApplicationWindow::initActions()");

    // Share menu among different windows on MAC
//#ifdef Q_OS_MAC
//    QMenuBar* mb = new QMenuBar(0);
//    setMenuBar(mb);
//#endif

    QMenu* menu = NULL;
    QMenu* subMenu = NULL;
    QToolBar* tb = NULL;

    // File actions:
    //
    menu = new QMenu(this);
    menu->setTitle(tr("&File"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(this);
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

    // Edit actions + HTML actions (separate toolbar):
    //
    menu = new QMenu(this);
    menu->setTitle(tr("&Edit"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(this);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_edit");
    tb->setWindowTitle(tr("Edit"));
    addToolBar(Qt::TopToolBarArea, tb);
    DB_ACTIONFACTORY->getEditMenu(menu, tb);
    tb->hide();

    // Format actions:
    //
    menu = new QMenu(this);
    menu->setTitle(tr("For&mat"));
    menuBar()->addMenu(menu);
    tb = new QToolBar(this);
    tb->setIconSize(QSize(16,16));
    tb->setObjectName("toolbar_format");
    tb->setWindowTitle(tr("Format"));
    addToolBar(Qt::TopToolBarArea, tb);
    DB_ACTIONFACTORY->getFormatMenu(menu, tb);
    tb->hide();

    // Plugin/perspective actions:
    //
    // ... such as ACC accounting, CRM customer relation management

    // Perspective (plugin) actions:
    //
    menu = new QMenu(tr("Pe&rspective"), this);
    menuBar()->addMenu(menu);
    DB_ACTIONFACTORY->getPerspectiveMenu(menu, NULL);

    // Window actions:
    //
    initWindowActions();

    // Setting actions: TODO:
    //
    // ... such as onetimedialog, connect directly to last database

    // System actions:
    //
    menu = new QMenu(tr("&Tools"), this);
    menuBar()->addMenu(menu);
    DB_ACTIONFACTORY->getToolsMenu(menu, NULL);

    // System actions:
    //
    menu = new QMenu(tr("S&ystem"), this);
    menuBar()->addMenu(menu);
    DB_ACTIONFACTORY->getSystemMenu(menu, NULL);

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

void BIL_MainWindow::initViews() {

    // ...

    RB_DEBUG->print("  command widget..");
    RB_DockWidget* dw = new RB_DockWidget(this, NULL);
    RB_Widget* wgt = DB_DIALOGFACTORY->getWidget(DB_DialogFactory::WidgetCommand, dw);
    dw->setWidget(wgt);
    dw->setAttribute(Qt::WA_DeleteOnClose, false); // otherwise perhaps true
//    connect(this, SIGNAL(windowsChanged(bool)), wgt, SLOT(setEnabled(bool)));
    dw->setMinimumHeight(30);
    dw->setWindowTitle(wgt->getName());
    dw->setObjectName("dockwidget_commandline");
    addDockWidget(Qt::BottomDockWidgetArea, dw);
    dw->hide();
}

/**
 * Initialize debug settings, such as signal spy target
 */
void BIL_MainWindow::initDebug() {
    // this->mSignalSpyTarget = this->mMdiArea;
}

/**
 * Slot showing the Biluna dasboard text MDI window
 */
void BIL_MainWindow::slotDashboard() {
//    RB_SETTINGS->beginGroup("DB_GettingStartedWidget");
//    int doNotShowAgain = RB_SETTINGS->value("donotshowagain", 0).toInt();
//    RB_SETTINGS->endGroup();
//    if (doNotShowAgain == 1) return;

    RB_MdiWindow* mdiW = DB_DIALOGFACTORY->getMdiWindow(DB_DialogFactory::WidgetCalendar);
    mdiW->setWindowTitle(tr("Biluna Dashboard"));
    mdiW->show();
    mdiW->raise();
}

/**
 * Slot handling signal from dashboard anchor has been clicked
 * @param url
 */
void BIL_MainWindow::slotDashboardAnchorClicked(const QUrl& url) {
    this->slotHelpSubject(url.toString());
}

/**
 * Demo quit slot, called from main.cpp
 */
void BIL_MainWindow::slotFileDemoQuit() {
    DB_DIALOGFACTORY->commandMessage(tr("Exiting application ..."));
    DB_DIALOGFACTORY->requestInformationDialog(
                "This is a <i>demo</i> version which terminates\n"
                "after 10 minutes. This version is not intended\n"
                "for any use. Please buy a full version from\n"
                "from the website for a small license fee.");
    qApp->exit(0);
}

void BIL_MainWindow::changeEvent(QEvent* e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
//        retranslateUi(this);
        break;
    default:
        break;
    }
}

