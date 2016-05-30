/*****************************************************************
 * $Id: rb_mainwindow.cpp 2235 2015-05-05 19:53:22Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_mainwindow.h"

#include "db_aboutdialog.h"
#include "db_actionfactory.h"
#include "db_actionsystemdatabaseconnect.h"
#include "db_dialogfactory.h"
//#include "db_filterxml.h"
#include "db_modelfactory.h"
#include "db_plugintesterdialog.h"
#include "db_utilityfactory.h"
#include "rb_actiondebugexportmodel.h"
#include "rb_actiondebugplugin.h"
#include "rb_actiondebugsignalspy.h"
#include "rb_actionfactory.h"
#include "rb_database.h"
#include "rb_debug.h"
#include "rb_dialogfactory.h"
#include "rb_guiaction.h"
#include "rb_mdiwindow.h"
#include "rb_modelfactory.h"
#include "rb_widget.h"
#include "rb_settings.h"
#include "rb_system.h"
#include "rb_xmlreader.h"
#include "rb_xmlwriter.h"

#ifdef DB_TEST
#include "db_testactionfactory.h"
#endif

#ifdef debug
#include "rb_signalspydialog.h"
#endif


/**
 * Constructor
 */
RB_MainWindow::RB_MainWindow(QWidget* parent, Qt::WindowFlags flags)
        : QMainWindow(parent, flags) {
    RB_DEBUG->print("RB_MainWindow::RB_MainWindow()");

    // Basic factories, ModelFactory also sets root when object factory is set
    DB_ModelFactory::getInstance(this);
    DB_DialogFactory::getInstance(this);
    DB_ActionFactory::getInstance(this);
    DB_UtilityFactory::getInstance();
#ifdef DB_TEST
    DB_TestActionFactory::getInstance(this);
#endif


    mWindowMenu = NULL;
    mEdition = "";
    mModules.clear();
    mHelpProcess = NULL;
    mSignalSpyTarget = NULL;

    mMdiArea = new QMdiArea(this);
    mMdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mMdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mMdiArea->setActivationOrder(QMdiArea::StackingOrder);
    mMdiArea->setTabsClosable(true);
    setCentralWidget(mMdiArea);
    setWindowIcon(QIcon(":/images/biluna16.png"));

    // Update menus based on type of activated window
    connect(mMdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(slotUpdateMenus(QMdiSubWindow*)));

    // Connect signals from different sources (MDI windows) to one slot or signal
    mWindowMapper = new QSignalMapper(this);
    connect(mWindowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(slotSetActiveSubWindow(QWidget*)));

    readSettings();
    statusBar()->showMessage("Ready", 4000);
}

/**
 * Destructor
 */
RB_MainWindow::~RB_MainWindow() {
#ifdef DB_TEST
    delete DB_TESTACTIONFACTORY;
#endif
    // These factories are always created and are part of the data model DB
    delete DB_UTILITYFACTORY;
    delete DB_ACTIONFACTORY;
    delete DB_DIALOGFACTORY; // will delete corresponding models
    delete DB_MODELFACTORY;  // will delete modelfactories and remaining models

    // Delete all MDI windows, which in turn will delete the relevant model
    // the remaining multi-puprose models are deleted by the modelFactory
    foreach (QMdiSubWindow* window, getMdiArea()->subWindowList()) {
        delete window;
    }

    if (mHelpProcess) {
        mHelpProcess->terminate();
    }

    // test possible memory leakage
    RB_DEBUG->printMemberCreated();
    RB_DEBUG->printMemberDeleted();
    RB_DEBUG->printObjectCreated();
    RB_DEBUG->printObjectDeleted();
    RB_DEBUG->printObjectList();
    RB_DEBUG->printMessages();

    RB_DEBUG->print("RB_MainWindow::~RB_MainWindow() OK");
}

QMdiArea* RB_MainWindow::getMdiArea() {
    return mMdiArea;
}

/**
 * File new slot, clears current in-memory model
 */
void RB_MainWindow::slotFileNew() {
    // nothing
}

/**
 * File open, loads file in current in-memory model only
 */
bool RB_MainWindow::slotFileOpen() {
    return true;
}

/**
 * File save model as XML file slot
 */
bool RB_MainWindow::slotFileSave() {
    return true;
}

/**
 * File save as slot
 */
bool RB_MainWindow::slotFileSaveAs(const RB_String& /* fileName */) {
    return true;
}

/**
 * To be overridden by implementation
 * Create and save PDF file
 */
void RB_MainWindow::slotFilePdf() {
    QMessageBox::warning(this, tr("Warning"), tr("Function not active."));
}

/**
 * To be overridden by implementation
 * Print data of active MDI window
 * refer also http://doc.trolltech.com/qq/qq27-odfwriter.html
 */
void RB_MainWindow::slotFilePrint() {
    QMessageBox::warning(this, tr("Warning"), tr("Function not active."));
}

/**
 * To be overridden by implementation
 * Preview print data of active MDI window
 */
void RB_MainWindow::slotFilePrintPreview(bool) {
    QMessageBox::warning(this, tr("Warning"), tr("Function not active."));
}

/**
 * File quit application slot
 */
void RB_MainWindow::slotFileQuit() {
    RB_DEBUG->print("RB_MainWindow::slotFileQuit()");

    // slotMessage(tr("Exiting application..."));
    close(); // triggers closeEvent()
}

/**
 * Set active mdi window based on 'mapped' signal
 * of mWindowMapper (=QSignalMapper)
 * @param wgt sub window
 */
void RB_MainWindow::slotSetActiveSubWindow(QWidget* window) {
    RB_DEBUG->print("RB_MainWindow::slotSetActiveSubWindow()");

    QMdiSubWindow* subWindow = qobject_cast<QMdiSubWindow*>(window);

    if (subWindow) {
        mMdiArea->setActiveSubWindow(subWindow);
    }

//    slotUpdateMenus(subWindow);
}

/**
 * Update all menus except 'Windows' menu
 */
void RB_MainWindow::slotUpdateMenus(QMdiSubWindow* w) {
    RB_DEBUG->print("RB_MainWindow::slotUpdateMenus()");

    RB_MdiWindow* mdiWindow = dynamic_cast<RB_MdiWindow*>(w);
    if (mdiWindow) {
        // emit type of widget now active
        emit subWindowChanged(mdiWindow->getWidgetType());
        emit subWindowActive(RB2::ValidTrue);
    } else {
        // no active subwindow widget
        emit subWindowChanged(DB_DialogFactory::WidgetNone);
        emit subWindowActive(RB2::ValidFalse);
    }
}

/**
 * Update window after MDI or Dock window(s) changed
 */
void RB_MainWindow::slotUpdateWindowMenu() {
    RB_DEBUG->print("RB_MainWindow::updateWindowMenu()");

    if (!mWindowMenu) return;

    mWindowMenu->clear();

    QMenu* subMenu = createPopupMenu();
    subMenu->setTitle(tr("&Toolbars"));
    mWindowMenu->addMenu(subMenu);

    mWindowMenu->addSeparator();

    mWindowMenu->addAction(mWindowCloseAction);
    mWindowMenu->addAction(mWindowCloseAllAction);
    mWindowMenu->addSeparator();
    mWindowMenu->addAction(mWindowCascadeAction);
    mWindowMenu->addAction(mWindowTileAction);
    mWindowMenu->addAction(mWindowTabAction);
    mWindowMenu->addSeparator();
    mWindowMenu->addAction(mWindowNextAction);
    mWindowMenu->addAction(mWindowPreviousAction);
    mWindowMenu->addAction(mWindowSeparatorAction);

    QList<QMdiSubWindow*> windows = mMdiArea->subWindowList();
    mWindowSeparatorAction->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        RB_MdiWindow* child = dynamic_cast<RB_MdiWindow*>(windows.at(i));
        RB_String text = "";

        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(child->getName());
        } else {
            text = tr("%1 %2").arg(i + 1).arg(child->getName());
        }

        QAction* a = mWindowMenu->addAction(text);
        a->setCheckable(true);
        a->setChecked(child == mMdiArea->activeSubWindow());

        // document selected from menu->Window
        connect(a, SIGNAL(triggered()), mWindowMapper, SLOT(map()));
        mWindowMapper->setMapping(a, windows.at(i));
    }
}

/**
 * About the application help slot
 */
void RB_MainWindow::slotHelpAbout() {
    // QMessageBox::aboutQt(this, "Biluna");

    QString modulesString;

    if (mModules.empty()==false) {
        modulesString = mModules.join(", ");
    } else {
        modulesString = tr("None");
    }

    DB_AboutDialog dlg(this);
    dlg.setWindowTitle(tr("About ..."));
    QString str(QString("<qt>") +
        "<h2>"+ RB_SYSTEM->getAppName() + "</h2>" +
        tr("<b>Version: </b> %1").arg(RB_SYSTEM->getAppVersion()) + "&nbsp;" +
#ifdef BILUNA_DEMO
        QString("[Demo]") + "<br>" +
#else
        QString("%1").arg(mEdition) + "<br>" +
#endif
        tr("<b>Compilation date:</b> %1").arg(__DATE__) +
        QString(", &copy;") + QString(" by Biluna<br>&nbsp;") +
        "<br>" +
        QString("<a href=\"http://www.biluna.com\">http://www.biluna.com</a>") +
        "<br />" +
        tr("<b>Perspectives:</b> <i>%1</i><br />").arg(modulesString)
        + tr("<b>Qt Version:</b> %1").arg(qVersion())
    );
    dlg.setText(str);
    dlg.setWindowIcon(QIcon(":/images/biluna32.png"));
    dlg.exec();
}

/**
 * Index of help slot
 */
void RB_MainWindow::slotHelpIndex() {
#ifdef Q_OS_MAC
    QDesktopServices::openUrl(QUrl("http://www.biluna.com/doc/help/index.html"));
    return; // HACK
#endif

    if (!mHelpProcess) {
        mHelpProcess = new QProcess(this);
        QStringList args;
        args << QLatin1String("-collectionFile")
                << QLatin1String("doc/doc.qhc")
                << QLatin1String("-enableRemoteControl");
#if defined(Q_OS_WIN)
        mHelpProcess->start(QLatin1String("assistant"), args);
#elif defined(Q_OS_MAC)
        QDir helpDir;
        helpDir.setPath(qApp->applicationDirPath());

        if (helpDir.dirName() == "MacOS") {
            helpDir.cdUp();
            helpDir.cdUp();
            helpDir.cdUp();
        }

        mHelpProcess->start(helpDir.absolutePath().toLatin1() + QLatin1String("/Assistant"), args);
#endif
        if (!mHelpProcess->waitForStarted()) {
            DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Could not start the Help browser"));
            return;
        } else {
            connect(mHelpProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
                    this, SLOT(slotHelpFinished(int,QProcess::ExitStatus)));
        }
    }
}

/**
 * Subject help slot
 * @param subject help subject, usually a widget object/class name
 */
void RB_MainWindow::slotHelpSubject(const RB_String& subject) {
#if defined(Q_OS_MAC)
    QDesktopServices::openUrl(QUrl("http://www.biluna.com/doc/help/"
                                   + subject.toLower() + ".html")); // HACK
    return;
#else
    // Ensure that Assistant has been started
    slotHelpIndex();

    RB_String sourcePath = "setSource qthelp://com.";
    RB_String str = RB_SETTINGS->getCompanyKey();
    sourcePath += str.toLower() + ".";
    str = RB_SYSTEM->getAppVersion();
    str.replace(".", "_");
    sourcePath += str + "/doc/"; // virtual folder in Doxygen, http://doc.trolltech.com/qthelpproject.html#virtual-folders
    RB_String fn = subject.toLower() + ".html";
    QByteArray ba;
    ba.append(sourcePath + fn + "\n");
    // example ba.append("setSource qthelp://com.mycompany.1_0_0/doc/index.html\n");

    mHelpProcess->write(ba);
#endif
}

/**
 * Credits help slot
 */
void RB_MainWindow::slotHelpCredits() {
    DB_AboutDialog dlg(this);
    dlg.setWindowTitle(tr("Credits ..."));
    QString str(QString("<qt>") +
        "<h2>"+ RB_SYSTEM->getAppName() + "</h2>"
        + tr("<b>Version:</b> %1").arg(RB_SYSTEM->getAppVersion()) + "<br />"
        + tr("<b>Icons:</b> ")
        + QString("Mark James <a href=\"http://www.famfamfam.com/lab/icons/silk/\">Silk icon set 1.3</a><br />")
        + QString("<b>Calendar:</b> Jukka-Pekka Makela jpm@mehteenit.net<br />")
        + QString("<b>Calculator:</b> <a href=\"http://www.speedcrunch.org\">Speedcrunch 0.10</a><br />")
    );
    dlg.setText(str);
    dlg.setWindowIcon(QIcon(":/images/biluna32.png"));
    dlg.exec();
}

/**
 * Slot help assistant was closed
 */
void RB_MainWindow::slotHelpFinished(int, QProcess::ExitStatus) {
    delete mHelpProcess;
    mHelpProcess = NULL;
}

/**
 * For debugging of plugins
 */
void RB_MainWindow::slotDebugPlugins() {
   DB_PluginTesterDialog dlg(this);
   dlg.exec();
}

/**
 * For debugging only of signal/slot only
 * @param target QObject or QWidget to be debugged
 */
void RB_MainWindow::slotDebugSignalSpy() {
#ifdef debug
    if (!mSignalSpyTarget) return;
    // Setup the spy, second paramenter is the widget (or QObject)
    // to be checked for the signal and slots
    RB_SignalSpyDialog* dlg = new RB_SignalSpyDialog(this, mSignalSpyTarget);
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->show(); // modeless instead of exec()
#endif
}

/**
 * For debug only of model, writes in-memory model to text edit window
 */
void RB_MainWindow::slotDebugExportModel() {
#ifdef debug
    // old
//    RB_String str = "";
//    writeModel(str, mModel->getRoot(), true);
//    this->textEdit->append(str);
#endif
}

/**
 * Handle actions before closing the application or due to clicking
 * the right top corner, in a dialog this is the reject() function.
 * @param event
 */
void RB_MainWindow::closeEvent(QCloseEvent* event) {
    RB_DEBUG->print("RB_MainWindow::closeEvent()");
    DB_DIALOGFACTORY->setMainWindowCloseEvent();

    bool closeAll = true;

    foreach (QMdiSubWindow* window, getMdiArea()->subWindowList()) {
        RB_MdiWindow* mdiWin = dynamic_cast<RB_MdiWindow*>(window);
        if (!mdiWin->getWidget()->closeWidget()) closeAll = false;
    }

    if (closeAll) {
        // All MDI window closeEvents were accepted
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

/**
 * Create window menu actions
 */
void RB_MainWindow::initWindowActions() {
    RB_DEBUG->print("RB_MainWindow::initWindowActions()");

    // Menu is created in slotUpdateWindowMenu();
    // mWindowCloseAction, etc. required due to slotUpdateWindowMenu()

    mWindowMenu = new QMenu(tr("&Window"), this);
    menuBar()->addMenu(mWindowMenu);

    QMenu* subMenu = createPopupMenu();
    subMenu->setTitle(tr("&Toolbars"));
    mWindowMenu->addMenu(subMenu);

    mWindowMenu->addSeparator();

    // Standard MDI window actions
    RB_GuiAction* ga;
    ga = mWindowCloseAction = new RB_GuiAction("Close window", tr("Cl&ose"));
    ga->setShortcut(tr("Ctrl+F4"));
    ga->setStatusTip(tr("Close the active window"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            mMdiArea, SLOT(closeActiveSubWindow()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowCloseAction, SLOT(slotSetEnabled(int)));

    ga = mWindowCloseAllAction = new RB_GuiAction("Close all windows", tr("Close &All"));
    ga->setStatusTip(tr("Close all the windows"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            mMdiArea, SLOT(closeAllSubWindows()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowCloseAllAction, SLOT(slotSetEnabled(int)));

    ga = mWindowCascadeAction = new RB_GuiAction("Cascade windows", tr("&Cascade"));
    ga->setStatusTip(tr("Cascade the windows"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            this, SLOT(cascadeSubWindows()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowCascadeAction, SLOT(slotSetEnabled(int)));

    ga = mWindowTileAction = new RB_GuiAction("Tile windows",tr("&Tile"));
    ga->setStatusTip(tr("Tile the windows"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            this, SLOT(tileSubWindows()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowTileAction, SLOT(slotSetEnabled(int)));

    ga = mWindowTabAction = new RB_GuiAction("Tabbed windows",tr("Ta&bbed"));
    ga->setStatusTip(tr("Tab the windows"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            this, SLOT(tabSubWindows()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowTabAction, SLOT(slotSetEnabled(int)));

    ga = mWindowNextAction = new RB_GuiAction("Next window", tr("Ne&xt"));
    ga->setStatusTip(tr("Move the focus to the next window"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            mMdiArea, SLOT(activateNextSubWindow()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowNextAction, SLOT(slotSetEnabled(int)));

    ga = mWindowPreviousAction = new RB_GuiAction("Previous window", tr("Pre&vious"));
    ga->setStatusTip(tr("Move the focus to the previous window"));
    ga->setEnabled(false);
    connect(ga, SIGNAL(triggered()),
            mMdiArea, SLOT(activatePreviousSubWindow()));
    connect(this, SIGNAL(subWindowActive(int)),
            mWindowPreviousAction, SLOT(slotSetEnabled(int)));

    QAction* a = mWindowSeparatorAction = new QAction(this);
    a->setSeparator(true);

    slotUpdateWindowMenu();
    connect(mWindowMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotUpdateWindowMenu()));
}

/**
 * @return active MDI window
 */
RB_MdiWindow* RB_MainWindow::getActiveMdiWindow() {
    if (QMdiSubWindow* activeSubWindow = mMdiArea->activeSubWindow()) {
        return qobject_cast<RB_MdiWindow*>(activeSubWindow);
    }
    return NULL;
}

/**
 * Set active MDI window
 * @param w existing MDI window
 */
void RB_MainWindow::setActiveMdiWindow(RB_MdiWindow* w) {
    this->slotSetActiveSubWindow(w);
}

/**
 * Find MDI window based on database ID, file name or window name
 * and (optional) widget type
 * @return MDI window or NULL if not found
 */
RB_MdiWindow* RB_MainWindow::findMdiWindow(const RB_String& idFileName,
                                           int widgetType) {
    if (idFileName.isEmpty() && widgetType < 0) {
        return NULL;
    }

    RB_String mdiWindowId = QFileInfo(idFileName).canonicalFilePath();

    if (mdiWindowId.isEmpty()) {
        // ID from database or window name
        mdiWindowId = idFileName;
    }

    foreach (QMdiSubWindow* subWindow, mMdiArea->subWindowList()) {
        RB_MdiWindow* mdiWindow = qobject_cast<RB_MdiWindow*>(subWindow);
        if (widgetType > 0) {
            if ((mdiWindow->getId() == mdiWindowId
                    || mdiWindow->getSaveAsFileName() == mdiWindowId)
                    && widgetType == mdiWindow->getWidgetType()) {
                return mdiWindow;
            }
        } else if (mdiWindow->getId() == mdiWindowId
                   || mdiWindow->getSaveAsFileName() == mdiWindowId) {
            return mdiWindow;
        }
    }
    return NULL;
}

/**
 * Read window position and size settings
 */
void RB_MainWindow::readSettings() {
    RB_SETTINGS->beginGroup(objectName());
    QPoint pos = RB_SETTINGS->value("pos", QPoint(100, 100)).toPoint();
    QSize size = RB_SETTINGS->value("size", QSize(800, 640)).toSize();
    RB_String viewMode = RB_SETTINGS->value("mdiarea", "subwindow").toString();
    RB_SETTINGS->endGroup();

    // Prevent opening position on second screen,
    // otherwise not visible via (single) desktop sharing
    int x = 0 <= pos.x() && pos.x() < 800 ? pos.x() : 100;
    int y = 0 <= pos.y() && pos.y() < 640 ? pos.y() : 100;
    pos.setX(x);
    pos.setY(y);

    resize(size);
    move(pos);
    if (viewMode == "tabbedview") {
        tabSubWindows();
    }
}

/**
 * Write window position and size settings
 */
void RB_MainWindow::writeSettings() {
    RB_SETTINGS->beginGroup(objectName());
    RB_SETTINGS->setValue("pos", pos());
    RB_SETTINGS->setValue("size", size());
    if (mMdiArea->viewMode() == QMdiArea::TabbedView) {
        RB_SETTINGS->setValue("mdiarea", "tabbedview");
    } else {
        RB_SETTINGS->setValue("mdiarea", "subwindow");
    }
    RB_SETTINGS->endGroup();
}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 */
bool RB_MainWindow::maybeSave() {
    return true;
}

/**
 * Load file in model
 * @param model model to be loaded with file
 * @param fileName name of file
 */
bool RB_MainWindow::loadFile(RB_ModelFactory* model,
                             const RB_String &fileName, const RB_String& fileType) {
    RB_DEBUG->print("RB_MainWindow::loadFile()");

    if (!model || fileName.isEmpty() || !isValidFileType(fileType)) return false;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Load file"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName) .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_XmlReader* xr = new RB_XmlReader(model->getRoot());
    xr->setNewId(false);
    bool success = xr->read(&file);

    QApplication::restoreOverrideCursor();

    if (!success) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Reading error %1:\n%2 (2).")
                    .arg(fileName)
                    .arg(xr->errorString()));
        statusBar()->showMessage(tr("Error loading file"), 2000);
    } else {
        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    delete xr;
    file.close();
    return true;
}

/**
 * Save model to file
 * @param model model to be saved
 * @param fileName name of file
 */
bool RB_MainWindow::saveModel(RB_ModelFactory* model,
                              const RB_String &fileName) {
    RB_DEBUG->print("RB_MainWindow::saveModel()");

    if (!model || fileName.isEmpty()) return false;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save file"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName) .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_XmlWriter* xw = new RB_XmlWriter(&file);
    model->getRoot()->acceptVisitor(*xw);
    delete xw;

    QApplication::restoreOverrideCursor();

    model->setModelIsModified(false);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Slot cascade MDI subWindows
 */
void RB_MainWindow::cascadeSubWindows() {
    mMdiArea->setViewMode(QMdiArea::SubWindowView);
    mMdiArea->cascadeSubWindows();
}

/**
 * Slot show tabbed MDI subWindows
 */
void RB_MainWindow::tabSubWindows() {
    mMdiArea->setViewMode(QMdiArea::TabbedView); // Default: QMdiArea::SubWindowView
    //    mMdiArea->setTabPosition(QTabWidget::West); // Default: QTabWidget::North
    //    mMdiArea->setTabShape(QTabWidget::Triangular); // Default: QTabWidget::Rounded
}

/**
 * Slot tile MDI subWindows
 */
void RB_MainWindow::tileSubWindows() {
    mMdiArea->setViewMode(QMdiArea::SubWindowView);
    mMdiArea->tileSubWindows();
}

/**
 * Close all MDI windows, i.e. if project selected in database changed,
 * also deletes the window
 */
void RB_MainWindow::closeAllMdiWindows() {
    foreach (QMdiSubWindow* subWindow, mMdiArea->subWindowList()) {
        RB_MdiWindow* mdiWindow = qobject_cast<RB_MdiWindow*>(subWindow);
        mdiWindow->close();
    }
    /* Above to close all relevant models
    mMdiArea->closeAllSubWindows();
    QList<QMdiSubWindow*> list = mMdiArea->subWindowList();
    list.clear();
    */
}

/**
 * Close all dockwidgets, i.e. if project selected in database changed,
 * does not delete the dockwidget.
 */
void RB_MainWindow::closeAllDockWidgets() {
    foreach (QObject* object, this->children()) {
        QDockWidget* dw = qobject_cast<QDockWidget*>(object);
        if (dw) {
            dw->close();
        }
    }
}

/**
 * Set signal spy target, for debugging only. The action
 * signal spy is disabled unless in debug mode
 */
void RB_MainWindow::setSignalSpyTarget(QObject* target) {
    mSignalSpyTarget = target;
}
