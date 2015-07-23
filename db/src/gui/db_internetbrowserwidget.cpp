/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "db_internetbrowserwidget.h"

#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"

#include "autosaver.h"
#include "bookmarks.h"
#include "chasewidget.h"
#include "downloadmanager.h"
#include "history.h"
#include "settings.h"
#include "tabwidget.h"
#include "toolbarsearch.h"
#include "ui_passworddialog.h"
#include "webview.h"

#include <QtCore/QSettings>

#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QPlainTextEdit>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QInputDialog>

#include <QtWebKitWidgets/QWebFrame>
#include <QWebHistory>

#include <QtCore/QDebug>

DB_InternetBrowserWidget::DB_InternetBrowserWidget(QWidget *parent, Qt::WindowFlags flags)
                        : RB_Widget(parent, flags)
    , m_tabWidget(new TabWidget(this))
    , m_autoSaver(new AutoSaver(this))
    , m_historyBack(0)
    , m_historyForward(0)
    , m_stop(0)
    , m_reload(0)
{

    setupUi(this);

//    setToolButtonStyle(Qt::ToolButtonFollowStyle);
//    setAttribute(Qt::WA_DeleteOnClose, true);
//    statusBar()->setSizeGripEnabled(true);
    setupMenu();
    setupToolBar();

//    QWidget *centralWidget = new QWidget(this);
//    BookmarksModel *boomarksModel = DB_InternetBrowserFactory::bookmarksManager()->bookmarksModel();
//    m_bookmarksToolbar = new BookmarksToolBar(boomarksModel, this);
//    connect(m_bookmarksToolbar, SIGNAL(openUrl(QUrl)),
//            m_tabWidget, SLOT(loadUrlInCurrentTab(QUrl)));
//    connect(m_bookmarksToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
//            this, SLOT(updateBookmarksToolbarActionText(bool)));

//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->setSpacing(0);
//    layout->setMargin(0);
//#if defined(Q_WS_MAC)
//    layout->addWidget(m_bookmarksToolbar);
//    layout->addWidget(new QWidget); // <- OS X tab widget style bug
//#else
//    addToolBarBreak();
//    addToolBar(m_bookmarksToolbar);
//#endif
//    layout->addWidget(m_tabWidget);
//    centralWidget->setLayout(layout);
//	setCentralWidget(centralWidget);
    gridLayout->addWidget(m_tabWidget, 1, 0, 1, 1);

    connect(m_tabWidget, SIGNAL(loadPage(QString)),
        this, SLOT(loadPage(QString)));
    connect(m_tabWidget, SIGNAL(setCurrentTitle(QString)),
        this, SLOT(slotUpdateWindowTitle(QString)));
    connect(m_tabWidget, SIGNAL(showStatusBarMessage(QString)),
            this, SLOT(slotUpdateStatusBar(QString)));
    connect(m_tabWidget, SIGNAL(linkHovered(QString)),
            this, SLOT(slotUpdateStatusBar(QString)));
    connect(m_tabWidget, SIGNAL(loadProgress(int)),
            this, SLOT(slotLoadProgress(int)));
    connect(m_tabWidget, SIGNAL(tabsChanged()),
            m_autoSaver, SLOT(changeOccurred()));
    connect(m_tabWidget, SIGNAL(geometryChangeRequested(QRect)),
            this, SLOT(geometryChangeRequested(QRect)));
    connect(m_tabWidget, SIGNAL(printRequested(QWebFrame*)),
            this, SLOT(printRequested(QWebFrame*)));
//    connect(m_tabWidget, SIGNAL(menuBarVisibilityChangeRequested(bool)),
//            menuBar(), SLOT(setVisible(bool)));
//    connect(m_tabWidget, SIGNAL(statusBarVisibilityChangeRequested(bool)),
//            statusBar(), SLOT(setVisible(bool)));
//    connect(m_tabWidget, SIGNAL(toolBarVisibilityChangeRequested(bool)),
//            m_navigationBar, SLOT(setVisible(bool)));
//    connect(m_tabWidget, SIGNAL(toolBarVisibilityChangeRequested(bool)),
//            m_bookmarksToolbar, SLOT(setVisible(bool)));
#if defined(Q_WS_MAC)
    connect(m_tabWidget, SIGNAL(lastTabClosed()),
            this, SLOT(close()));
#else
    connect(m_tabWidget, SIGNAL(lastTabClosed()),
            m_tabWidget, SLOT(newTab()));
#endif

//    slotUpdateWindowTitle();
//    loadDefaultState();
    m_tabWidget->newTab();

//    int size = m_tabWidget->lineEditStack()->sizeHint().height();
//    m_navigationBar->setIconSize(QSize(size, size));

    mWidgetName = tr("Web Viewer");
}

DB_InternetBrowserWidget::~DB_InternetBrowserWidget()
{
//    m_autoSaver->changeOccurred();
//    m_autoSaver->saveIfNeccessary();
}

/**
 * @return database document ID
 */
RB_String DB_InternetBrowserWidget::getId() const {
    return RB_String();
}

/**
 * Get name for MDI window title
 * @return file name or type of window (in case of unique window)
 */
RB_String DB_InternetBrowserWidget::getName() const {
    return mWidgetName;
}

/**
 * File export PDF.
 * @param printer
 */
void DB_InternetBrowserWidget::filePdf(QPrinter* printer) {
    currentTab()->print(printer);
}

/**
 * File Print.
 * @param printer
 */
void DB_InternetBrowserWidget::filePrint(QPrinter* printer) {
    currentTab()->print(printer);
}

/**
 * File Print Preview.
 * @param printer
 */
void DB_InternetBrowserWidget::filePrintPreview(QPrinter* printer) {
    currentTab()->print(printer);
}


/*
void DB_InternetBrowserWidget::loadDefaultState()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("DB_InternetBrowserWidget"));
    QByteArray data = settings.value(QLatin1String("defaultState")).toByteArray();
    restoreState(data);
    settings.endGroup();
}
*/
QSize DB_InternetBrowserWidget::sizeHint() const
{
//    QRect desktopRect = QApplication::desktop()->screenGeometry();
    QRect mdiAreaRect = DB_DIALOGFACTORY->getMainWindow()->getMdiArea()->geometry();
    QSize size = mdiAreaRect.size() * qreal(0.9);
    return size;
}

void DB_InternetBrowserWidget::save()
{
    DB_InternetBrowserFactory::getInstance()->saveSession();

    QSettings settings;
    settings.beginGroup(QLatin1String("DB_InternetBrowserWidget"));
    QByteArray data = saveState(false);
    settings.setValue(QLatin1String("defaultState"), data);
    settings.endGroup();
}

static const qint32 BrowserMainWindowMagic = 0xba;

QByteArray DB_InternetBrowserWidget::saveState(bool withTabs) const
{
    int version = 2;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << qint32(BrowserMainWindowMagic);
    stream << qint32(version);

    stream << size();
//    stream << !m_navigationBar->isHidden();
//    stream << !m_bookmarksToolbar->isHidden();
//    stream << !statusBar()->isHidden();
    if (withTabs)
        stream << tabWidget()->saveState();
    else
        stream << QByteArray();
    return data;
}

bool DB_InternetBrowserWidget::restoreState(const QByteArray &state)
{
    int version = 2;
    QByteArray sd = state;
    QDataStream stream(&sd, QIODevice::ReadOnly);
    if (stream.atEnd())
        return false;

    qint32 marker;
    qint32 v;
    stream >> marker;
    stream >> v;
    if (marker != BrowserMainWindowMagic || v != version)
        return false;

    QSize size;
//    bool showToolbar;
//    bool showBookmarksBar;
//    bool showStatusbar;
    QByteArray tabState;

    stream >> size;
//    stream >> showToolbar;
//    stream >> showBookmarksBar;
//    stream >> showStatusbar;
    stream >> tabState;

    resize(size);

//    m_navigationBar->setVisible(showToolbar);
//    updateToolbarActionText(showToolbar);

//    m_bookmarksToolbar->setVisible(showBookmarksBar);
//    updateBookmarksToolbarActionText(showBookmarksBar);

//    statusBar()->setVisible(showStatusbar);
//    updateStatusbarActionText(showStatusbar);

    if (!tabWidget()->restoreState(tabState))
        return false;

    return true;
}

void DB_InternetBrowserWidget::setupMenu()
{
//    new QShortcut(QKeySequence(Qt::Key_F6), this, SLOT(slotSwapFocus()));

    QMenu* menu = new QMenu(tr("&Menu"), this);
    tbMenu->setMenu(menu);
    tbMenu->setIcon(QIcon(":/images/icons/menu.png"));

    // File
//    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* fileMenu = new QMenu(tr("&File"), this);
    menu->addMenu(fileMenu);

//    fileMenu->addAction(tr("&New Window"), this, SLOT(slotFileNew()), QKeySequence::New);
    fileMenu->addAction(m_tabWidget->newTabAction());
    fileMenu->addAction(tr("&Open File..."), this, SLOT(slotFileOpen()), QKeySequence::Open);
    fileMenu->addAction(tr("Open &Location..."), this,
                SLOT(slotSelectLineEdit()), QKeySequence(Qt::ControlModifier + Qt::Key_L));
    fileMenu->addSeparator();
    fileMenu->addAction(m_tabWidget->closeTabAction());
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Save As..."), this,
                SLOT(slotFileSaveAs()), QKeySequence(QKeySequence::Save));
    fileMenu->addSeparator();
    BookmarksManager *bookmarksManager = DB_InternetBrowserFactory::bookmarksManager();
    fileMenu->addAction(tr("&Import Bookmarks..."), bookmarksManager, SLOT(importBookmarks()));
    fileMenu->addAction(tr("&Export Bookmarks..."), bookmarksManager, SLOT(exportBookmarks()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("P&rint Preview..."), this, SLOT(slotFilePrintPreview()));
    fileMenu->addAction(tr("&Print..."), this, SLOT(slotFilePrint()));
    fileMenu->addSeparator();
    QAction *action = fileMenu->addAction(tr("Private &Browsing..."), this, SLOT(slotPrivateBrowsing()));
    action->setCheckable(true);
//    fileMenu->addSeparator();

//#if defined(Q_WS_MAC)
//    fileMenu->addAction(tr("&Quit"), DB_InternetBrowserFactory::getInstance(), SLOT(quitBrowser()), QKeySequence(Qt::CTRL | Qt::Key_Q));
//#else
//    fileMenu->addAction(tr("&Quit"), this, SLOT(close()), QKeySequence(Qt::CTRL | Qt::Key_Q));
//#endif

    // Edit
//    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu* editMenu = new QMenu(tr("&Edit"), this);
    menu->addMenu(editMenu);

    QAction *m_undo = editMenu->addAction(tr("&Undo"));
    m_undo->setShortcuts(QKeySequence::Undo);
    m_tabWidget->addWebAction(m_undo, QWebPage::Undo);
    QAction *m_redo = editMenu->addAction(tr("&Redo"));
    m_redo->setShortcuts(QKeySequence::Redo);
    m_tabWidget->addWebAction(m_redo, QWebPage::Redo);
    editMenu->addSeparator();
    QAction *m_cut = editMenu->addAction(tr("Cu&t"));
    m_cut->setShortcuts(QKeySequence::Cut);
    m_tabWidget->addWebAction(m_cut, QWebPage::Cut);
    QAction *m_copy = editMenu->addAction(tr("&Copy"));
    m_copy->setShortcuts(QKeySequence::Copy);
    m_tabWidget->addWebAction(m_copy, QWebPage::Copy);
    QAction *m_paste = editMenu->addAction(tr("&Paste"));
    m_paste->setShortcuts(QKeySequence::Paste);
    m_tabWidget->addWebAction(m_paste, QWebPage::Paste);
    editMenu->addSeparator();

    QAction *m_find = editMenu->addAction(tr("&Find"));
    m_find->setShortcuts(QKeySequence::Find);
    connect(m_find, SIGNAL(triggered()), this, SLOT(slotEditFind()));
    new QShortcut(QKeySequence(Qt::Key_Slash), this, SLOT(slotEditFind()));

    QAction *m_findNext = editMenu->addAction(tr("&Find Next"));
    m_findNext->setShortcuts(QKeySequence::FindNext);
    connect(m_findNext, SIGNAL(triggered()), this, SLOT(slotEditFindNext()));

    QAction *m_findPrevious = editMenu->addAction(tr("&Find Previous"));
    m_findPrevious->setShortcuts(QKeySequence::FindPrevious);
    connect(m_findPrevious, SIGNAL(triggered()), this, SLOT(slotEditFindPrevious()));

    editMenu->addSeparator();
    editMenu->addAction(tr("&Preferences"), this, SLOT(slotPreferences()), tr("Ctrl+,"));

    // View
//    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QMenu* viewMenu = new QMenu(tr("&View"), this);
    menu->addMenu(viewMenu);

//    m_viewBookmarkBar = new QAction(this);
//    updateBookmarksToolbarActionText(true);
//    m_viewBookmarkBar->setShortcut(tr("Shift+Ctrl+B"));
//    connect(m_viewBookmarkBar, SIGNAL(triggered()), this, SLOT(slotViewBookmarksBar()));
//    viewMenu->addAction(m_viewBookmarkBar);

//    m_viewToolbar = new QAction(this);
//    updateToolbarActionText(true);
//    m_viewToolbar->setShortcut(tr("Ctrl+|"));
//    connect(m_viewToolbar, SIGNAL(triggered()), this, SLOT(slotViewToolbar()));
//    viewMenu->addAction(m_viewToolbar);

//    m_viewStatusbar = new QAction(this);
//    updateStatusbarActionText(true);
//    m_viewStatusbar->setShortcut(tr("Ctrl+/"));
//    connect(m_viewStatusbar, SIGNAL(triggered()), this, SLOT(slotViewStatusbar()));
//    viewMenu->addAction(m_viewStatusbar);

//    viewMenu->addSeparator();

    m_stop = viewMenu->addAction(tr("&Stop"));
    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Period));
    shortcuts.append(Qt::Key_Escape);
    m_stop->setShortcuts(shortcuts);
    m_tabWidget->addWebAction(m_stop, QWebPage::Stop);

    m_reload = viewMenu->addAction(tr("Reload Page"));
    m_reload->setShortcuts(QKeySequence::Refresh);
    m_tabWidget->addWebAction(m_reload, QWebPage::Reload);

    viewMenu->addAction(tr("Zoom &In"), this, SLOT(slotViewZoomIn()), QKeySequence(Qt::CTRL | Qt::Key_Plus));
    viewMenu->addAction(tr("Zoom &Out"), this, SLOT(slotViewZoomOut()), QKeySequence(Qt::CTRL | Qt::Key_Minus));
    viewMenu->addAction(tr("Reset &Zoom"), this, SLOT(slotViewResetZoom()), QKeySequence(Qt::CTRL | Qt::Key_0));
    QAction *zoomTextOnlyAction = viewMenu->addAction(tr("Zoom &Text Only"));
    connect(zoomTextOnlyAction, SIGNAL(toggled(bool)), this, SLOT(slotViewZoomTextOnly(bool)));
    zoomTextOnlyAction->setCheckable(true);
    zoomTextOnlyAction->setChecked(false);

    viewMenu->addSeparator();
    viewMenu->addAction(tr("Page S&ource"), this, SLOT(slotViewPageSource()), tr("Ctrl+Alt+U"));
    QAction* a;
//    QAction *a = viewMenu->addAction(tr("&Full Screen"), this, SLOT(slotViewFullScreen(bool)),  Qt::Key_F11);
//    a->setCheckable(true);

    // History
    HistoryMenu *historyMenu = new HistoryMenu(this);
    connect(historyMenu, SIGNAL(openUrl(QUrl)),
            m_tabWidget, SLOT(loadUrlInCurrentTab(QUrl)));
    connect(historyMenu, SIGNAL(hovered(QString)), this,
            SLOT(slotUpdateStatusBar(QString)));
    historyMenu->setTitle(tr("Hi&story"));
//    menuBar()->addMenu(historyMenu);
    menu->addMenu(historyMenu);
    QList<QAction*> historyActions;

    m_historyBack = new QAction(tr("Back"), this);
    m_tabWidget->addWebAction(m_historyBack, QWebPage::Back);
    m_historyBack->setShortcuts(QKeySequence::Back);
    m_historyBack->setIconVisibleInMenu(false);

    m_historyForward = new QAction(tr("Forward"), this);
    m_tabWidget->addWebAction(m_historyForward, QWebPage::Forward);
    m_historyForward->setShortcuts(QKeySequence::Forward);
    m_historyForward->setIconVisibleInMenu(false);

    QAction *m_historyHome = new QAction(tr("Home"), this);
    connect(m_historyHome, SIGNAL(triggered()), this, SLOT(slotHome()));
    m_historyHome->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_H));

    m_restoreLastSession = new QAction(tr("Restore Last Session"), this);
    connect(m_restoreLastSession, SIGNAL(triggered()), DB_InternetBrowserFactory::getInstance(), SLOT(restoreLastSession()));
    m_restoreLastSession->setEnabled(DB_InternetBrowserFactory::getInstance()->canRestoreSession());

    historyActions.append(m_historyBack);
    historyActions.append(m_historyForward);
    historyActions.append(m_historyHome);
    historyActions.append(m_tabWidget->recentlyClosedTabsAction());
    historyActions.append(m_restoreLastSession);
    historyMenu->setInitialActions(historyActions);

    // Bookmarks
    BookmarksMenu *bookmarksMenu = new BookmarksMenu(this);
    connect(bookmarksMenu, SIGNAL(openUrl(QUrl)),
            m_tabWidget, SLOT(loadUrlInCurrentTab(QUrl)));
    connect(bookmarksMenu, SIGNAL(hovered(QString)),
            this, SLOT(slotUpdateStatusBar(QString)));
    bookmarksMenu->setTitle(tr("&Bookmarks"));
//    menuBar()->addMenu(bookmarksMenu);
    menu->addMenu(bookmarksMenu);

    QList<QAction*> bookmarksActions;

    QAction *showAllBookmarksAction = new QAction(tr("Show All Bookmarks"), this);
    connect(showAllBookmarksAction, SIGNAL(triggered()), this, SLOT(slotShowBookmarksDialog()));
    m_addBookmark = new QAction(QIcon(QLatin1String(":addbookmark.png")), tr("Add Bookmark..."), this);
    m_addBookmark->setIconVisibleInMenu(false);

    connect(m_addBookmark, SIGNAL(triggered()), this, SLOT(slotAddBookmark()));
    m_addBookmark->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));

    bookmarksActions.append(showAllBookmarksAction);
    bookmarksActions.append(m_addBookmark);
    bookmarksMenu->setInitialActions(bookmarksActions);

    // Window
//    m_windowMenu = menuBar()->addMenu(tr("&Window"));
    m_windowMenu = new QMenu(tr("&Window"), this);
    menu->addMenu(m_windowMenu);

    connect(m_windowMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotAboutToShowWindowMenu()));
    slotAboutToShowWindowMenu();

//    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    QMenu* toolsMenu = new QMenu(tr("&Tools"), this);
    menu->addMenu(toolsMenu);

    toolsMenu->addAction(tr("Web &Search"), this, SLOT(slotWebSearch()), QKeySequence(tr("Ctrl+K", "Web Search")));
    a = toolsMenu->addAction(tr("Enable Web &Inspector"), this, SLOT(slotToggleInspector(bool)));
    a->setCheckable(true);

//    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
//    QMenu* helpMenu = new QMenu(tr("&Help"), this);
//    menu->addMenu(helpMenu);

//    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
//    helpMenu->addAction(tr("About &Demo Browser"), this, SLOT(slotAboutApplication()));
}

void DB_InternetBrowserWidget::setupToolBar()
{
//    setUnifiedTitleAndToolBarOnMac(true);
//    m_navigationBar = addToolBar(tr("Navigation"));
//    connect(m_navigationBar->toggleViewAction(), SIGNAL(toggled(bool)),
//            this, SLOT(updateToolbarActionText(bool)));

    m_historyBack->setIcon(style()->standardIcon(QStyle::SP_ArrowBack, 0, this));
    m_historyBackMenu = new QMenu(this);
    m_historyBack->setMenu(m_historyBackMenu);
    connect(m_historyBackMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotAboutToShowBackMenu()));
    connect(m_historyBackMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(slotOpenActionUrl(QAction*)));
//    m_navigationBar->addAction(m_historyBack);
    tbBack->setDefaultAction(m_historyBack);
    tbBack->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    tbBack->setPopupMode(QToolButton::MenuButtonPopup);
    tbBack->setAutoRaise(true);

    m_historyForward->setIcon(style()->standardIcon(QStyle::SP_ArrowForward, 0, this));
    m_historyForwardMenu = new QMenu(this);
    m_historyForward->setMenu(m_historyForwardMenu);
    connect(m_historyForwardMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotAboutToShowForwardMenu()));
    connect(m_historyForwardMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(slotOpenActionUrl(QAction*)));
//    m_navigationBar->addAction(m_historyForward);
    tbForward->setDefaultAction(m_historyForward);
    tbForward->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    tbForward->setPopupMode(QToolButton::MenuButtonPopup);
    tbForward->setAutoRaise(true);

    m_stopReload = new QAction(this);
    m_reloadIcon = style()->standardIcon(QStyle::SP_BrowserReload);
//    m_stopReload->setIcon(m_reloadIcon);
//    m_navigationBar->addAction(m_stopReload);
    tbReload->setDefaultAction(m_stopReload);
    tbReload->setIcon(m_reloadIcon);
    tbReload->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tbReload->setPopupMode(QToolButton::DelayedPopup);
    tbReload->setAutoRaise(true);

//    m_navigationBar->addWidget(m_tabWidget->lineEditStack());
    horizontalLayout->addWidget(m_tabWidget->lineEditStack());

    m_toolbarSearch = new ToolbarSearch(this);
    connect(m_toolbarSearch, SIGNAL(search(QUrl)), SLOT(loadUrl(QUrl)));
//    m_navigationBar->addWidget(m_toolbarSearch);
    horizontalLayout->addWidget(m_toolbarSearch);

    m_chaseWidget = new ChaseWidget(this);
//    m_navigationBar->addWidget(m_chaseWidget);
    horizontalLayout->addWidget(m_chaseWidget);

}

void DB_InternetBrowserWidget::slotShowBookmarksDialog()
{
    BookmarksDialog *dialog = new BookmarksDialog(this);
    connect(dialog, SIGNAL(openUrl(QUrl)),
            m_tabWidget, SLOT(loadUrlInCurrentTab(QUrl)));
    dialog->show();
}

void DB_InternetBrowserWidget::slotAddBookmark()
{
    WebView *webView = currentTab();
    QString url = webView->url().toString();
    QString title = webView->title();
    AddBookmarkDialog dialog(url, title);
    dialog.exec();
}
/*
void DB_InternetBrowserWidget::slotViewToolbar()
{
    if (m_navigationBar->isVisible()) {
        updateToolbarActionText(false);
        m_navigationBar->close();
    } else {
        updateToolbarActionText(true);
        m_navigationBar->show();
    }
    m_autoSaver->changeOccurred();
}

void DB_InternetBrowserWidget::slotViewBookmarksBar()
{
    if (m_bookmarksToolbar->isVisible()) {
        updateBookmarksToolbarActionText(false);
        m_bookmarksToolbar->close();
    } else {
        updateBookmarksToolbarActionText(true);
        m_bookmarksToolbar->show();
    }
    m_autoSaver->changeOccurred();
}
*/
void DB_InternetBrowserWidget::updateStatusbarActionText(bool visible)
{
    m_viewStatusbar->setText(!visible ? tr("Show Status Bar") : tr("Hide Status Bar"));
}

void DB_InternetBrowserWidget::updateToolbarActionText(bool visible)
{
    m_viewToolbar->setText(!visible ? tr("Show Toolbar") : tr("Hide Toolbar"));
}

void DB_InternetBrowserWidget::updateBookmarksToolbarActionText(bool visible)
{
    m_viewBookmarkBar->setText(!visible ? tr("Show Bookmarks bar") : tr("Hide Bookmarks bar"));
}
/*
void DB_InternetBrowserWidget::slotViewStatusbar()
{
    if (statusBar()->isVisible()) {
        updateStatusbarActionText(false);
        statusBar()->close();
    } else {
        updateStatusbarActionText(true);
        statusBar()->show();
    }
    m_autoSaver->changeOccurred();
}
*/
void DB_InternetBrowserWidget::loadUrl(const QUrl &url)
{
    if (!currentTab() || !url.isValid())
        return;

    m_tabWidget->currentLineEdit()->setText(QString::fromUtf8(url.toEncoded()));
    m_tabWidget->loadUrlInCurrentTab(url);
}

void DB_InternetBrowserWidget::slotDownloadManager()
{
    DB_InternetBrowserFactory::downloadManager()->show();
}

void DB_InternetBrowserWidget::slotSelectLineEdit()
{
    m_tabWidget->currentLineEdit()->selectAll();
    m_tabWidget->currentLineEdit()->setFocus();
}

void DB_InternetBrowserWidget::slotFileSaveAs()
{
    DB_InternetBrowserFactory::downloadManager()->download(currentTab()->url(), true);
}

void DB_InternetBrowserWidget::slotPreferences()
{
//    SettingsDialog *s = new SettingsDialog(this);
//    s->show();
}

void DB_InternetBrowserWidget::slotUpdateStatusBar(const QString &string)
{
//    statusBar()->showMessage(string, 2000);
    DB_DIALOGFACTORY->statusBarMessage(string, 2000);
}

void DB_InternetBrowserWidget::slotUpdateWindowTitle(const QString &title)
{
    if (title.isEmpty()) {
//        setWindowTitle(tr("Qt Demo Browser"));
        mWidgetName = tr("Biluna Web Viewer");
    } else {
#if defined(Q_WS_MAC)
//        setWindowTitle(title);
        mWidgetName = title;
#else
//        setWindowTitle(tr("%1 - Qt Demo Browser", "Page title and Browser name").arg(title));
        mWidgetName = tr("%1 - Biluna Web Viewer", "Page title and Browser name").arg(title);
#endif
    }

    emit modificationChanged(false);
}

void DB_InternetBrowserWidget::slotAboutApplication()
{
    QMessageBox::about(this, tr("About"), tr(
        "Version %1"
        "<p>This demo demonstrates Qt's "
        "webkit facilities in action, providing an example "
        "browser for you to experiment with.<p>"
        "<p>QtWebKit is based on the Open Source WebKit Project developed at <a href=\"http://webkit.org/\">http://webkit.org/</a>."
        ).arg(QCoreApplication::applicationVersion()));
}
/*
void DB_InternetBrowserWidget::slotFileNew()
{
    DB_InternetBrowserFactory::getInstance()->newMainWindow();
    DB_InternetBrowserWidget *mw = DB_InternetBrowserFactory::getInstance()->mainWindow();
    mw->slotHome();
}
//*/
void DB_InternetBrowserWidget::slotFileOpen()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Web Resource"), QString(),
            tr("Web Resources (*.html *.htm *.svg *.png *.gif *.svgz);;All files (*.*)"));

    if (file.isEmpty())
        return;

    loadPage(file);
}

void DB_InternetBrowserWidget::slotFilePrintPreview()
{
#ifndef QT_NO_PRINTER
    if (!currentTab())
        return;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(this);
    connect(dialog, SIGNAL(paintRequested(QPrinter*)),
            currentTab(), SLOT(print(QPrinter*)));
    dialog->exec();
#endif
}

void DB_InternetBrowserWidget::slotFilePrint()
{
    if (!currentTab())
        return;
    printRequested(currentTab()->page()->mainFrame());
}

void DB_InternetBrowserWidget::printRequested(QWebFrame *frame)
{
#ifndef QT_NO_PRINTER
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    frame->print(&printer);
#endif
}

void DB_InternetBrowserWidget::slotPrivateBrowsing()
{
    QWebSettings *settings = QWebSettings::globalSettings();
    bool pb = settings->testAttribute(QWebSettings::PrivateBrowsingEnabled);
    if (!pb) {
        QString title = tr("Are you sure you want to turn on private browsing?");
        QString text = tr("<b>%1</b><br><br>When private browsing in turned on,"
            " webpages are not added to the history,"
            " items are automatically removed from the Downloads window," \
            " new cookies are not stored, current cookies can't be accessed," \
            " site icons wont be stored, session wont be saved, " \
            " and searches are not added to the pop-up menu in the Google search box." \
            "  Until you close the window, you can still click the Back and Forward buttons" \
            " to return to the webpages you have opened.").arg(title);

        QMessageBox::StandardButton button = QMessageBox::question(this, QString(), text,
                               QMessageBox::Ok | QMessageBox::Cancel,
                               QMessageBox::Ok);
        if (button == QMessageBox::Ok) {
            settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
        }
    } else {
        settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);

        QList<DB_InternetBrowserWidget*> windows =
                DB_InternetBrowserFactory::getInstance()->browserWidgets();
        for (int i = 0; i < windows.count(); ++i) {
            DB_InternetBrowserWidget *window = windows.at(i);
            window->m_lastSearch = QString::null;
            window->tabWidget()->clear();
        }
    }
}

void DB_InternetBrowserWidget::closeEvent(QCloseEvent *event)
{
    if (m_tabWidget->count() > 1) {
        int ret = QMessageBox::warning(this, QString(),
                           tr("Are you sure you want to close the window?"
                              "  There are %1 tabs open").arg(m_tabWidget->count()),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No);
        if (ret == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    event->accept();
//    deleteLater();
    closeWidget();
}

void DB_InternetBrowserWidget::slotEditFind()
{
    if (!currentTab())
        return;
    bool ok;
    QString search = QInputDialog::getText(this, tr("Find"),
                                          tr("Text:"), QLineEdit::Normal,
                                          m_lastSearch, &ok);
    if (ok && !search.isEmpty()) {
        m_lastSearch = search;
        if (!currentTab()->findText(m_lastSearch))
            slotUpdateStatusBar(tr("\"%1\" not found.").arg(m_lastSearch));
    }
}

void DB_InternetBrowserWidget::slotEditFindNext()
{
    if (!currentTab() && !m_lastSearch.isEmpty())
        return;
    currentTab()->findText(m_lastSearch);
}

void DB_InternetBrowserWidget::slotEditFindPrevious()
{
    if (!currentTab() && !m_lastSearch.isEmpty())
        return;
    currentTab()->findText(m_lastSearch, QWebPage::FindBackward);
}

void DB_InternetBrowserWidget::slotViewZoomIn()
{
    if (!currentTab())
        return;
    currentTab()->setZoomFactor(currentTab()->zoomFactor() + 0.1);
}

void DB_InternetBrowserWidget::slotViewZoomOut()
{
    if (!currentTab())
        return;
    currentTab()->setZoomFactor(currentTab()->zoomFactor() - 0.1);
}

void DB_InternetBrowserWidget::slotViewResetZoom()
{
    if (!currentTab())
        return;
    currentTab()->setZoomFactor(1.0);
}

void DB_InternetBrowserWidget::slotViewZoomTextOnly(bool enable)
{
    if (!currentTab())
        return;
    currentTab()->page()->settings()->setAttribute(QWebSettings::ZoomTextOnly, enable);
}

void DB_InternetBrowserWidget::slotViewFullScreen(bool makeFullScreen)
{
    if (makeFullScreen) {
        showFullScreen();
    } else {
        if (isMinimized())
            showMinimized();
        else if (isMaximized())
            showMaximized();
        else showNormal();
    }
}

void DB_InternetBrowserWidget::slotViewPageSource()
{
    if (!currentTab())
        return;

    QString markup = currentTab()->page()->mainFrame()->toHtml();
    QPlainTextEdit *view = new QPlainTextEdit(markup);
    view->setWindowTitle(tr("Page Source of %1").arg(currentTab()->title()));
    view->setMinimumWidth(640);
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->show();
}

void DB_InternetBrowserWidget::slotHome()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("MainWindow"));
    QString home = settings.value(QLatin1String("home"),
                                  QLatin1String("http://www.biluna.com/")).toString();
    loadPage(home);
}

void DB_InternetBrowserWidget::slotWebSearch()
{
    m_toolbarSearch->lineEdit()->selectAll();
    m_toolbarSearch->lineEdit()->setFocus();
}

void DB_InternetBrowserWidget::slotToggleInspector(bool enable)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, enable);
    if (enable) {
        int result = QMessageBox::question(this, tr("Web Inspector"),
                                           tr("The web inspector will only work correctly for pages that were loaded after enabling.\n"
                                           "Do you want to reload all pages?"),
                                           QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            m_tabWidget->reloadAllTabs();
        }
    }
}

void DB_InternetBrowserWidget::slotSwapFocus()
{
    if (currentTab()->hasFocus())
        m_tabWidget->currentLineEdit()->setFocus();
    else
        currentTab()->setFocus();
}

void DB_InternetBrowserWidget::loadPage(const QString &page)
{
    QUrl url = QUrl::fromUserInput(page);
    loadUrl(url);
}

TabWidget *DB_InternetBrowserWidget::tabWidget() const
{
    return m_tabWidget;
}

WebView *DB_InternetBrowserWidget::currentTab() const
{
    return m_tabWidget->currentWebView();
}

void DB_InternetBrowserWidget::slotLoadProgress(int progress)
{
    if (progress < 100 && progress > 0) {
        m_chaseWidget->setAnimated(true);
        disconnect(m_stopReload, SIGNAL(triggered()), m_reload, SLOT(trigger()));
        if (m_stopIcon.isNull())
            m_stopIcon = style()->standardIcon(QStyle::SP_BrowserStop);
        m_stopReload->setIcon(m_stopIcon);
        connect(m_stopReload, SIGNAL(triggered()), m_stop, SLOT(trigger()));
        m_stopReload->setToolTip(tr("Stop loading the current page"));
    } else {
        m_chaseWidget->setAnimated(false);
        disconnect(m_stopReload, SIGNAL(triggered()), m_stop, SLOT(trigger()));
        m_stopReload->setIcon(m_reloadIcon);
        connect(m_stopReload, SIGNAL(triggered()), m_reload, SLOT(trigger()));
        m_stopReload->setToolTip(tr("Reload the current page"));
    }
}

void DB_InternetBrowserWidget::slotAboutToShowBackMenu()
{
    m_historyBackMenu->clear();
    if (!currentTab())
        return;
    QWebHistory *history = currentTab()->history();
    int historyCount = history->count();
    for (int i = history->backItems(historyCount).count() - 1; i >= 0; --i) {
        QWebHistoryItem item = history->backItems(history->count()).at(i);
        QAction *action = new QAction(this);
        action->setData(-1*(historyCount-i-1));
        QIcon icon = DB_InternetBrowserFactory::getInstance()->icon(item.url());
        action->setIcon(icon);
        action->setText(item.title());
        m_historyBackMenu->addAction(action);
    }
}

void DB_InternetBrowserWidget::slotAboutToShowForwardMenu()
{
    m_historyForwardMenu->clear();
    if (!currentTab())
        return;
    QWebHistory *history = currentTab()->history();
    int historyCount = history->count();
    for (int i = 0; i < history->forwardItems(history->count()).count(); ++i) {
        QWebHistoryItem item = history->forwardItems(historyCount).at(i);
        QAction *action = new QAction(this);
        action->setData(historyCount-i);
        QIcon icon = DB_InternetBrowserFactory::getInstance()->icon(item.url());
        action->setIcon(icon);
        action->setText(item.title());
        m_historyForwardMenu->addAction(action);
    }
}

void DB_InternetBrowserWidget::slotAboutToShowWindowMenu()
{
    m_windowMenu->clear();
    m_windowMenu->addAction(m_tabWidget->nextTabAction());
    m_windowMenu->addAction(m_tabWidget->previousTabAction());
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(tr("Downloads"), this, SLOT(slotDownloadManager()), QKeySequence(tr("Alt+Ctrl+L", "Download Manager")));

    m_windowMenu->addSeparator();
    QList<DB_InternetBrowserWidget*> windows = DB_InternetBrowserFactory::getInstance()->browserWidgets();
    for (int i = 0; i < windows.count(); ++i) {
        DB_InternetBrowserWidget *window = windows.at(i);
        QAction *action = m_windowMenu->addAction(window->windowTitle(), this, SLOT(slotShowWindow()));
        action->setData(i);
        action->setCheckable(true);
        if (window == this)
            action->setChecked(true);
    }
}
/*
void DB_InternetBrowserWidget::slotShowWindow()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        QVariant v = action->data();
        if (v.canConvert<int>()) {
            int offset = qvariant_cast<int>(v);
            QList<DB_InternetBrowserWidget*> windows = DB_InternetBrowserFactory::getInstance()->mainWindows();
            windows.at(offset)->activateWindow();
            windows.at(offset)->currentTab()->setFocus();
        }
    }
}
*/
void DB_InternetBrowserWidget::slotOpenActionUrl(QAction *action)
{
    int offset = action->data().toInt();
    QWebHistory *history = currentTab()->history();
    if (offset < 0)
        history->goToItem(history->backItems(-1*offset).first()); // back
    else if (offset > 0)
        history->goToItem(history->forwardItems(history->count() - offset + 1).back()); // forward
 }

void DB_InternetBrowserWidget::geometryChangeRequested(const QRect &geometry)
{
    setGeometry(geometry);
}

