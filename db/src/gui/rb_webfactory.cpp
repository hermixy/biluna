/*****************************************************************
 * $Id: peng_actioneqtype.cpp 1370 2011-02-17 22:26:05Z rutger $
 * Created: Feb 28, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_webfactory.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QWebPluginFactory>
#include "rb_webwidget.h"


RB_WebFactory::RB_WebFactory(QObject* parent) : QWebPluginFactory(parent) {
    mManager = new QNetworkAccessManager(this);
}

QObject* RB_WebFactory::create(const QString& mimeType, const QUrl& /*url*/,
                            const QStringList& argumentNames,
                            const QStringList& argumentValues) const {
    if (mimeType != "image/in-memory")
        return 0;

/*
    RB_WebWidget* view = new RB_WebWidget(argumentValues[argumentNames.indexOf("type")]);

    QNetworkRequest request(url);
    QNetworkReply* reply = mManager->get(request);
    connect(reply, SIGNAL(finished()), view, SLOT(updateModel()));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));

    return view;
*/
    RB_WebWidget* wdgt = new RB_WebWidget(argumentValues[argumentNames.indexOf("type")]);
    wdgt->load(mSvg.toUtf8());
    wdgt->resize(400, 400);
    return wdgt;
}

QList<QWebPluginFactory::Plugin> RB_WebFactory::plugins() const {
    QWebPluginFactory::MimeType mimeType;
    mimeType.name = "image/in-memory";
    mimeType.description = "In-memory image";
    mimeType.fileExtensions = QStringList() << "svg";

    QWebPluginFactory::Plugin plugin;
    plugin.name = "SVG viewer";
    plugin.description = "A SVG file Web plugin.";
    plugin.mimeTypes = QList<MimeType>() << mimeType;

    return QList<QWebPluginFactory::Plugin>() << plugin;
}
