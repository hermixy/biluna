/*****************************************************************
 * $Id: peng_actioneqtype.cpp 1370 2011-02-17 22:26:05Z rutger $
 * Created: Feb 28, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_WEBFACTORY_H
#define RB_WEBFACTORY_H

#include <QNetworkRequest>
#include <QWebPluginFactory>
#include "db_global.h"
#include "rb_string.h"

class QNetworkAccessManager;
class QNetworkReply;

class DB_EXPORT RB_WebFactory : public QWebPluginFactory
{
    Q_OBJECT

public:
    RB_WebFactory(QObject* parent = 0);
    QObject* create(const QString& mimeType, const QUrl& url,
                    const QStringList& argumentNames,
                    const QStringList& argumentValues) const;
    QList<QWebPluginFactory::Plugin> plugins() const;

    void setSvg(const RB_String& svg) { mSvg = svg; }

private:
    QNetworkAccessManager* mManager;
    RB_String mSvg;
};

#endif // RB_WEBFACTORY_H
