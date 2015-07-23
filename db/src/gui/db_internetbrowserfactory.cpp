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

#include "db_internetbrowserfactory.h"

#include "db_dialogfactory.h"
#include "db_internetbrowserwidget.h"
#include "db_utilityfactory.h"
#include "rb_mdiwindow.h"

#include "bookmarks.h"
#include "cookiejar.h"
#include "downloadmanager.h"
#include "history.h"
#include "networkaccessmanager.h"
#include "tabwidget.h"
#include "webview.h"

#include <QtCore/QBuffer>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>

#include <QDesktopServices>
#include <QFileOpenEvent>
#include <QtWidgets/QMessageBox>

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QSslSocket>

#include <QtWebKit/QWebSettings>

#include <QtCore/QDebug>

DownloadManager *DB_InternetBrowserFactory::s_downloadManager = 0;
HistoryManager *DB_InternetBrowserFactory::s_historyManager = 0;
NetworkAccessManager *DB_InternetBrowserFactory::s_networkAccessManager = 0;
BookmarksManager *DB_InternetBrowserFactory::s_bookmarksManager = 0;
DB_InternetBrowserFactory* DB_InternetBrowserFactory::mActiveFactory = 0;


DB_InternetBrowserFactory::DB_InternetBrowserFactory(RB_MainWindow* mw)
    : QObject(), RB_UtilityFactory() /*: QApplication(argc, argv)*/
    , m_localServer(0), mMainWindow(mw)
{
    DB_UTILITYFACTORY->registerFactory(this);

//    QCoreApplication::setOrganizationName(QLatin1String("Qt"));
//    QCoreApplication::setApplicationName(QLatin1String("demobrowser"));
//    QCoreApplication::setApplicationVersion(QLatin1String("0.1"));
#ifdef Q_WS_QWS
    // Use a different server name for QWS so we can run an X11
    // browser and a QWS browser in parallel on the same machine for
    // debugging
    QString serverName = QCoreApplication::applicationName() + QLatin1String("_qws");
#else
    QString serverName = QCoreApplication::applicationName();
#endif
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500)) {
        QTextStream stream(&socket);
//        QStringList args = QCoreApplication::arguments();
//        if (args.count() > 1)
//            stream << args.last();
//        else
            stream << QString();
        stream.flush();
        socket.waitForBytesWritten();
        return;
    }

//#if defined(Q_WS_MAC)
//    QApplication::setQuitOnLastWindowClosed(false);
//#else
//    QApplication::setQuitOnLastWindowClosed(true);
//#endif

    m_localServer = new QLocalServer(this);
    connect(m_localServer, SIGNAL(newConnection()),
            this, SLOT(newLocalSocketConnection()));
    if (!m_localServer->listen(serverName)) {
        if (m_localServer->serverError() == QAbstractSocket::AddressInUseError
            && QFile::exists(m_localServer->serverName())) {
            QFile::remove(m_localServer->serverName());
            m_localServer->listen(serverName);
        }
    }

#ifndef QT_NO_OPENSSL
    if (!QSslSocket::supportsSsl()) {
        QMessageBox::information(0, "Internet Browser",
                                 "This system does not support OpenSSL. "
                                 "SSL websites will not be available.");
    }
#endif

    QDesktopServices::setUrlHandler(QLatin1String("http"), this, "openUrl");
    QString localSysName = QLocale::system().name();

    installTranslator(QLatin1String("qt_") + localSysName);

    QSettings settings;
    settings.beginGroup(QLatin1String("sessions"));
    m_lastSession = settings.value(QLatin1String("lastSession")).toByteArray();
    settings.endGroup();

#if defined(Q_WS_MAC)
    connect(this, SIGNAL(lastWindowClosed()),
            this, SLOT(lastWindowClosed()));
#endif

    QTimer::singleShot(0, this, SLOT(postLaunch()));
}

DB_InternetBrowserFactory::~DB_InternetBrowserFactory()
{
    delete s_downloadManager;
    // MDI windows are deleted by XXX_MainWindow
//    for (int i = 0; i < m_browserWidgets.size(); ++i) {
//        DB_InternetBrowserWidget *window = m_browserWidgets.at(i);
//        delete window;
//    }
    delete s_networkAccessManager;
    delete s_bookmarksManager;

    DB_UTILITYFACTORY->unregisterFactory(this);
    mActiveFactory = NULL;
}

#if defined(Q_WS_MAC)
void DB_InternetBrowserFactory::lastWindowClosed()
{
    clean();
    DB_InternetBrowserWidget *mw = new DB_InternetBrowserWidget;
    mw->slotHome();
    m_browserWidgets.prepend(mw);
}
#endif

DB_InternetBrowserFactory *DB_InternetBrowserFactory::getInstance(RB_MainWindow* mw) {
//    return (static_cast<DB_InternetBrowserFactory *>(QCoreApplication::instance()));
    if (!mActiveFactory) {
        mActiveFactory = new DB_InternetBrowserFactory(mw);
    }

    return mActiveFactory;
}

#if defined(Q_WS_MAC)
#include <QtWidgets/QMessageBox>
void DB_InternetBrowserFactory::quitBrowser()
{
    clean();
    int tabCount = 0;
    for (int i = 0; i < m_browserWidgets.count(); ++i) {
        tabCount =+ m_browserWidgets.at(i)->tabWidget()->count();
    }

    if (tabCount > 1) {
        int ret = QMessageBox::warning(browserWidget(), QString(),
                           tr("There are %1 windows and %2 tabs open\n"
                              "Do you want to quit anyway?").arg(m_browserWidgets.count()).arg(tabCount),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No);
        if (ret == QMessageBox::No)
            return;
    }

    exit(0);
}
#endif


// Any actions that can be delayed until the window is visible
void DB_InternetBrowserFactory::postLaunch()
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (directory.isEmpty())
        directory = QDir::homePath() + QLatin1String("/.") + QCoreApplication::applicationName();
    QWebSettings::setIconDatabasePath(directory);
    QWebSettings::setOfflineStoragePath(directory);

//    setWindowIcon(QIcon(QLatin1String(":browser.svg")));

    loadSettings();

    // newBrowserWidget() needs to be called in main() for this to happen
    if (m_browserWidgets.count() > 0) {
        QStringList args = QCoreApplication::arguments();
        if (args.count() > 1)
            browserWidget()->loadPage(args.last());
        else
            browserWidget()->slotHome();
    }
    DB_InternetBrowserFactory::historyManager();
}

void DB_InternetBrowserFactory::loadSettings()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("websettings"));

    QWebSettings *defaultSettings = QWebSettings::globalSettings();
    QString standardFontFamily = defaultSettings->fontFamily(QWebSettings::StandardFont);
    int standardFontSize = defaultSettings->fontSize(QWebSettings::DefaultFontSize);
    QFont standardFont = QFont(standardFontFamily, standardFontSize);
    // standardFont = qVariantValue<QFont>(settings.value(QLatin1String("standardFont"), standardFont));
    standardFont = settings.value(QLatin1String("standardFont"), standardFont).value<QFont>();
    defaultSettings->setFontFamily(QWebSettings::StandardFont, standardFont.family());
    defaultSettings->setFontSize(QWebSettings::DefaultFontSize, standardFont.pointSize());

    QString fixedFontFamily = defaultSettings->fontFamily(QWebSettings::FixedFont);
    int fixedFontSize = defaultSettings->fontSize(QWebSettings::DefaultFixedFontSize);
    QFont fixedFont = QFont(fixedFontFamily, fixedFontSize);
    // fixedFont = qVariantValue<QFont>(settings.value(QLatin1String("fixedFont"), fixedFont));
    fixedFont = settings.value(QLatin1String("fixedFont"), fixedFont).value<QFont>();
    defaultSettings->setFontFamily(QWebSettings::FixedFont, fixedFont.family());
    defaultSettings->setFontSize(QWebSettings::DefaultFixedFontSize, fixedFont.pointSize());

    defaultSettings->setAttribute(QWebSettings::JavascriptEnabled, settings.value(QLatin1String("enableJavascript"), true).toBool());
    defaultSettings->setAttribute(QWebSettings::PluginsEnabled, settings.value(QLatin1String("enablePlugins"), true).toBool());

    QUrl url = settings.value(QLatin1String("userStyleSheet")).toUrl();
    defaultSettings->setUserStyleSheetUrl(url);

    defaultSettings->setAttribute(QWebSettings::DnsPrefetchEnabled, true);

    settings.endGroup();
}

QList<DB_InternetBrowserWidget*> DB_InternetBrowserFactory::browserWidgets()
{
    clean();
    QList<DB_InternetBrowserWidget*> list;
    for (int i = 0; i < m_browserWidgets.count(); ++i)
        list.append(m_browserWidgets.at(i));
    return list;
}

void DB_InternetBrowserFactory::clean()
{
    // cleanup any deleted main windows first
    for (int i = m_browserWidgets.count() - 1; i >= 0; --i)
        if (m_browserWidgets.at(i).isNull())
            m_browserWidgets.removeAt(i);
}

void DB_InternetBrowserFactory::saveSession()
{
    QWebSettings *globalSettings = QWebSettings::globalSettings();
    if (globalSettings->testAttribute(QWebSettings::PrivateBrowsingEnabled))
        return;

    clean();

    QSettings settings;
    settings.beginGroup(QLatin1String("sessions"));

    QByteArray data;
    QBuffer buffer(&data);
    QDataStream stream(&buffer);
    buffer.open(QIODevice::ReadWrite);

    stream << m_browserWidgets.count();
    for (int i = 0; i < m_browserWidgets.count(); ++i)
        stream << m_browserWidgets.at(i)->saveState();
    settings.setValue(QLatin1String("lastSession"), data);
    settings.endGroup();
}

bool DB_InternetBrowserFactory::canRestoreSession() const
{
    return !m_lastSession.isEmpty();
}

void DB_InternetBrowserFactory::restoreLastSession()
{
    QList<QByteArray> windows;
    QBuffer buffer(&m_lastSession);
    QDataStream stream(&buffer);
    buffer.open(QIODevice::ReadOnly);
    int windowCount;
    stream >> windowCount;
    for (int i = 0; i < windowCount; ++i) {
        QByteArray windowState;
        stream >> windowState;
        windows.append(windowState);
    }
    for (int i = 0; i < windows.count(); ++i) {
        DB_InternetBrowserWidget *newWindow = 0;
        if (m_browserWidgets.count() == 1
            && browserWidget()->tabWidget()->count() == 1
            && browserWidget()->currentTab()->url() == QUrl()) {
            newWindow = browserWidget();
        } else {
            newWindow = newBrowserWidget();
        }
        newWindow->restoreState(windows.at(i));
    }
}

bool DB_InternetBrowserFactory::isTheOnlyBrowser() const
{
    return (m_localServer != 0);
}

void DB_InternetBrowserFactory::installTranslator(const QString &name)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(name, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QApplication::installTranslator(translator);
}

#if defined(Q_WS_MAC)
bool DB_InternetBrowserFactory::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::ApplicationActivate: {
        clean();
        if (!m_browserWidgets.isEmpty()) {
            DB_InternetBrowserWidget *mw = browserWidget();
            if (mw && !mw->isMinimized()) {
                browserWidget()->show();
            }
            return true;
        }
    }
    case QEvent::FileOpen:
        if (!m_browserWidgets.isEmpty()) {
            browserWidget()->loadPage(static_cast<QFileOpenEvent *>(event)->file());
            return true;
        }
    default:
        break;
    }
    return QApplication::event(event);
}
#endif

void DB_InternetBrowserFactory::openUrl(const QUrl &url)
{
    browserWidget()->loadPage(url.toString());
}

DB_InternetBrowserWidget* DB_InternetBrowserFactory::newBrowserWidget()
{
//    DB_InternetBrowserWidget *browser = new DB_InternetBrowserWidget();
//    m_browserWidgets.prepend(browser);
//    browser->show();
//    return browser;

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                DB_DialogFactory::WidgetInternet);
    DB_InternetBrowserWidget* wdgt =
            dynamic_cast<DB_InternetBrowserWidget*>(mdiWin->getWidget());
    m_browserWidgets.prepend(wdgt);
    mdiWin->show();
    return wdgt;
}

DB_InternetBrowserWidget *DB_InternetBrowserFactory::browserWidget()
{
    clean();
    if (m_browserWidgets.isEmpty())
        newBrowserWidget();
    return m_browserWidgets[0];
}

void DB_InternetBrowserFactory::newLocalSocketConnection()
{
    QLocalSocket *socket = m_localServer->nextPendingConnection();
    if (!socket)
        return;
    socket->waitForReadyRead(1000);
    QTextStream stream(socket);
    QString url;
    stream >> url;
    if (!url.isEmpty()) {
        QSettings settings;
        settings.beginGroup(QLatin1String("general"));
        int openLinksIn = settings.value(QLatin1String("openLinksIn"), 0).toInt();
        settings.endGroup();
        if (openLinksIn == 1)
            newBrowserWidget();
        else
            browserWidget()->tabWidget()->newTab();
        openUrl(url);
    }
    delete socket;
    browserWidget()->raise();
    browserWidget()->activateWindow();
}

CookieJar *DB_InternetBrowserFactory::cookieJar()
{
    return (CookieJar*)networkAccessManager()->cookieJar();
}

DownloadManager *DB_InternetBrowserFactory::downloadManager()
{
    if (!s_downloadManager) {
        s_downloadManager = new DownloadManager();
    }
    return s_downloadManager;
}

NetworkAccessManager *DB_InternetBrowserFactory::networkAccessManager()
{
    if (!s_networkAccessManager) {
        s_networkAccessManager = new NetworkAccessManager();
        s_networkAccessManager->setCookieJar(new CookieJar);
    }
    return s_networkAccessManager;
}

HistoryManager *DB_InternetBrowserFactory::historyManager()
{
    if (!s_historyManager) {
        s_historyManager = new HistoryManager();
        QWebHistoryInterface::setDefaultInterface(s_historyManager);
    }
    return s_historyManager;
}

BookmarksManager *DB_InternetBrowserFactory::bookmarksManager()
{
    if (!s_bookmarksManager) {
        s_bookmarksManager = new BookmarksManager;
    }
    return s_bookmarksManager;
}

QIcon DB_InternetBrowserFactory::icon(const QUrl &url) const
{
    QIcon icon = QWebSettings::iconForUrl(url);
    if (!icon.isNull())
        return icon.pixmap(16, 16);
    if (m_defaultIcon.isNull())
        m_defaultIcon = QIcon(QLatin1String(":defaulticon.png"));
    return m_defaultIcon.pixmap(16, 16);
}

