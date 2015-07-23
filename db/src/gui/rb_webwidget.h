/*****************************************************************
 * $Id: peng_actioneqtype.cpp 1370 2011-02-17 22:26:05Z rutger $
 * Created: Feb 28, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_WEBWIDGET_H
#define RB_WEBWIDGET_H

#include <QNetworkRequest>
#include <QSvgWidget>
#include <QWebPluginFactory>
#include "db_global.h"

class QNetworkAccessManager;
class QNetworkReply;

class DB_EXPORT RB_WebWidget : public QSvgWidget {

    Q_OBJECT

public:
    RB_WebWidget(const QString& mimeType, QWidget* parent = 0);

public slots:
//    void updateModel();

private:
//    void addRow(bool firstLine, QStandardItemModel *model,
//                const QList<QStandardItem *> &items);

    QString mMimeType;
};

#endif
