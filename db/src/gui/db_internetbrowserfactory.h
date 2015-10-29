/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DB_INTERNETBROWSERFACTORY_H
#define DB_INTERNETBROWSERFACTORY_H

//#include <QtWidgets/QApplication>

#include <QtCore/QUrl>
#include <QtCore/QPointer>
#include <QtGui/QIcon>
#include "db_global.h"
#include "rb_mainwindow.h"
#include "rb_utilityfactory.h"

QT_BEGIN_NAMESPACE
class QLocalServer;
class QNetworkAccessManager;
class QWebEngineProfile;
QT_END_NAMESPACE

class BookmarksManager;
// class BrowserMainWindow;
class DB_InternetBrowserWidget;
class CookieJar;
class DownloadManager;
class HistoryManager;

#define DB_INTERNETBROWSERFACTORY DB_InternetBrowserFactory::getInstance()

//class BrowserApplication : public QApplication
class DB_EXPORT DB_InternetBrowserFactory : public QObject, RB_UtilityFactory
{
    Q_OBJECT

public:
//    BrowserApplication(int &argc, char **argv);
//    ~BrowserApplication();
//    static BrowserApplication *instance();
    DB_InternetBrowserFactory(RB_MainWindow* mw);
    ~DB_InternetBrowserFactory();
    static DB_InternetBrowserFactory* getInstance(RB_MainWindow* mw = 0);
    void loadSettings();

    bool isTheOnlyBrowser() const;
//    BrowserMainWindow *mainWindow();
//    QList<BrowserMainWindow*> mainWindows();
    DB_InternetBrowserWidget* browserWidget();
    QList<DB_InternetBrowserWidget*> browserWidgets();

    QIcon icon(const QUrl &url) const;
    QIcon defaultIcon() const;

    void saveSession();
    bool canRestoreSession() const;
    bool privateBrowsing() const { return m_privateBrowsing; }

    static HistoryManager *historyManager();
    static CookieJar *cookieJar();
    static DownloadManager *downloadManager();
    static QNetworkAccessManager *networkAccessManager();
    static BookmarksManager *bookmarksManager();

#if defined(Q_OS_OSX)
    bool event(QEvent *event);
#endif

public slots:
//    BrowserMainWindow *newMainWindow();
    DB_InternetBrowserWidget *newBrowserWidget();
    void restoreLastSession();
#if defined(Q_OS_OSX)
    void lastWindowClosed();
    void quitBrowser();
#endif
    void setPrivateBrowsing(bool);

signals:
    void privateBrowsingChanged(bool);

private slots:
    void postLaunch();
    void openUrl(const QUrl &url);
    void newLocalSocketConnection();

private:
    void clean();
    void installTranslator(const QString &name);

    static HistoryManager *s_historyManager;
    static DownloadManager *s_downloadManager;
    static QNetworkAccessManager *s_networkAccessManager;
    static BookmarksManager *s_bookmarksManager;

//    QList<QPointer<BrowserMainWindow> > m_mainWindows;
    QList<QPointer<DB_InternetBrowserWidget> > m_browserWidgets;
    QLocalServer *m_localServer;
    QByteArray m_lastSession;
    QWebEngineProfile *m_privateProfile;
    bool m_privateBrowsing;
    mutable QIcon m_defaultIcon;

    static DB_InternetBrowserFactory* mActiveFactory;
    RB_MainWindow* mBrowserWidget;
};

#endif // DB_INTERNETBROWSERFACTORY_H

