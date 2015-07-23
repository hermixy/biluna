/*****************************************************************
 * $Id: db_connectionwidget.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#ifndef DB_CONNECTIONWIDGET_H
#define DB_CONNECTIONWIDGET_H

#include <qwidget.h>
#include "db_global.h"

class QTreeWidget;
class QTreeWidgetItem;
class QSqlDatabase;
class QMenu;

class DB_EXPORT DB_ConnectionWidget: public QWidget {
	
    Q_OBJECT
    
public:
    DB_ConnectionWidget(QWidget* parent = 0);
    virtual ~DB_ConnectionWidget();

    QSqlDatabase currentDatabase() const;
    QString currentDatabaseName() const;
    QString currentTableName() const;

signals:
    void tableActivated(const QString& table);
    void metaDataRequested(const QString &tableName);
    void tableClicked(const QString& table);

public slots:
    void refresh();
    void showMetaData();
    void on_tree_itemActivated(QTreeWidgetItem* item, int column);
    void on_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_tree_itemClicked(QTreeWidgetItem* item, int column);

private:
    void setActive(QTreeWidgetItem* item);

    QTreeWidget* tree;
    QAction* metaDataAction;
    QString currentDb;
    QString currentTblName;
};

#endif
