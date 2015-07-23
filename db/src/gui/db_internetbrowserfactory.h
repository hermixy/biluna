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

#ifndef DB_INTERNETBROWSERFACTORY_H
#define DB_INTERNETBROWSERFACTORY_H

//#include <QtWidgets/QApplication>

#include <QtCore/QUrl>
#include <QtCore/QPointer>
#include <QIcon>
#include "db_global.h"
#include "rb_mainwindow.h"
#include "rb_utilityfactory.h"

QT_BEGIN_NAMESPACE
class QLocalServer;
QT_END_NAMESPACE

class BookmarksManager;
class DB_InternetBrowserWidget;
class CookieJar;
class DownloadManager;
class HistoryManager;
class NetworkAccessManager;

#define DB_INTERNETBROWSERFACTORY DB_InternetBrowserFactory::getInstance()


class DB_EXPORT DB_InternetBrowserFactory : public QObject, RB_UtilityFactory /*: public QApplication*/
{
    Q_OBJECT

public:
    DB_InternetBrowserFactory(RB_MainWindow* mw);
    ~DB_InternetBrowserFactory();
    static DB_InternetBrowserFactory* getInstance(RB_MainWindow* mw = 0);
    void loadSettings();

    bool isTheOnlyBrowser() const;
    DB_InternetBrowserWidget* browserWidget();
    QList<DB_InternetBrowserWidget*> browserWidgets();
    QIcon icon(const QUrl &url) const;

    void saveSession();
    bool canRestoreSession() const;

    static HistoryManager *historyManager();
    static CookieJar *cookieJar();
    static DownloadManager *downloadManager();
    static NetworkAccessManager *networkAccessManager();
    static BookmarksManager *bookmarksManager();

#if defined(Q_WS_MAC)
    bool event(QEvent *event);
#endif

public slots:
    DB_InternetBrowserWidget *newBrowserWidget();
    void restoreLastSession();
#if defined(Q_WS_MAC)
    void lastWindowClosed();
    void quitBrowser();
#endif

private slots:
    void postLaunch();
    void openUrl(const QUrl &url);
    void newLocalSocketConnection();

private:
    void clean();
    void installTranslator(const QString &name);

    static HistoryManager *s_historyManager;
    static DownloadManager *s_downloadManager;
    static NetworkAccessManager *s_networkAccessManager;
    static BookmarksManager *s_bookmarksManager;

    QList<QPointer<DB_InternetBrowserWidget> > m_browserWidgets;
    QLocalServer *m_localServer;
    QByteArray m_lastSession;
    mutable QIcon m_defaultIcon;

    static DB_InternetBrowserFactory* mActiveFactory;
    RB_MainWindow* mMainWindow;
};

#endif // DB_INTERNETBROWSERFACTORY_H

