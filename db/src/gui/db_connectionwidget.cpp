/*****************************************************************
 * $Id: db_connectionwidget.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "db_connectionwidget.h"

#include <QtWidgets>
#include <QtSql>
#include "rb_debug.h"


DB_ConnectionWidget::DB_ConnectionWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    tree = new QTreeWidget(this);
    tree->setObjectName(QLatin1String("tree"));
    tree->setHeaderLabels(QStringList(tr("Database/Tables")));
    tree->header()->setSectionResizeMode(QHeaderView::Stretch);
    
    QAction* refreshAction = new QAction(tr("Refresh"), tree);
    connect(refreshAction, SIGNAL(triggered()), SLOT(refresh()));
    tree->addAction(refreshAction);

    metaDataAction = new QAction(tr("Show Schema"), tree);
    connect(metaDataAction, SIGNAL(triggered()), SLOT(showMetaData()));
    tree->addAction(metaDataAction);
    tree->setContextMenuPolicy(Qt::ActionsContextMenu);

    QLabel* label = new QLabel(this);
    label->setText("Double click item to select");
    layout->addWidget(label);
    layout->addWidget(tree);

    QMetaObject::connectSlotsByName(this);
    
    currentTblName = "";
}

DB_ConnectionWidget::~DB_ConnectionWidget() {

}

static QString qDBCaption(const QSqlDatabase &db) {
    QString nm = db.driverName();
    nm.append(QLatin1Char(':'));
    if (!db.userName().isEmpty()) {
        nm.append(db.userName()).append(QLatin1Char('@'));
    }
    nm.append(db.databaseName());
    // test only
    nm.append(QLatin1Char(':'));
    nm.append(db.connectionName());
    return nm;
}

void DB_ConnectionWidget::refresh() {
    RB_DEBUG->print("DB_ConnectionWidget::refresh()");

    tree->clear();
    QStringList connectionNames = QSqlDatabase::connectionNames();
    currentTblName = "";

    bool gotCurrentDb = false;
    for (int i = 0; i < connectionNames.count(); ++i) {
        QTreeWidgetItem *root = new QTreeWidgetItem(tree);
        QSqlDatabase db = QSqlDatabase::database(connectionNames.at(i), false);
        root->setText(0, qDBCaption(db));
        if (connectionNames.at(i) == currentDb) {
            gotCurrentDb = true;
            setActive(root);
        }
        if (db.isOpen()) {
            QStringList tables = db.tables();
            tables.sort();

            for (int t = 0; t < tables.count(); ++t) {
                QTreeWidgetItem *table = new QTreeWidgetItem(root);
                table->setText(0, tables.at(t));
            }
        }
    }
    if (!gotCurrentDb) {
        currentDb = connectionNames.value(0);
        setActive(tree->topLevelItem(0));
    }

    tree->doItemsLayout(); // HACK
}

QSqlDatabase DB_ConnectionWidget::currentDatabase() const {
    return QSqlDatabase::database(currentDb);
}

QString DB_ConnectionWidget::currentDatabaseName() const {
	return currentDb;
}

QString DB_ConnectionWidget::currentTableName() const {
	return currentTblName;
}

static void qSetBold(QTreeWidgetItem *item, bool bold) {
    QFont font = item->font(0);
    font.setBold(bold);
    item->setFont(0, font);
}

void DB_ConnectionWidget::setActive(QTreeWidgetItem* item) {
	RB_DEBUG->print("DB_ConnectionWidget::setActive()");
	
    for (int i = 0; i < tree->topLevelItemCount(); ++i) {
        if (tree->topLevelItem(i)->font(0).bold())
            qSetBold(tree->topLevelItem(i), false);
    }

    if (!item) {
        return;
    }
    
    qSetBold(item, true);
    currentDb = QSqlDatabase::connectionNames().value(tree->indexOfTopLevelItem(item));
}

void DB_ConnectionWidget::on_tree_itemActivated(QTreeWidgetItem *item, int/*column*/) {
	RB_DEBUG->print("DB_ConnectionWidget::on_tree_itemActivated()");
	
    if (!item) {
        return;
    }
    if (!item->parent()) {
        setActive(item);
    } else {
        setActive(item->parent());
        emit tableActivated(item->text(0));
    }
}

void DB_ConnectionWidget::showMetaData() {
	RB_DEBUG->print("DB_ConnectionWidget::showMetaData()");
    QTreeWidgetItem *cItem = tree->currentItem();
    if (!cItem || !cItem->parent())
        return;
    setActive(cItem->parent());
    emit metaDataRequested(cItem->text(0));
}

void DB_ConnectionWidget::on_tree_currentItemChanged(QTreeWidgetItem *current, 
		QTreeWidgetItem *) {
	RB_DEBUG->print("DB_ConnectionWidget::on_tree_currentItemChanged()");
	
    metaDataAction->setEnabled(current && current->parent());

    // set table name if applicable
    if (current && current->parent()) {
    	currentTblName = current->text(0);
    } else {
    	currentTblName = "";
    }
}

/**
 * signal for DbMappingDialog
 */
void DB_ConnectionWidget::on_tree_itemClicked(QTreeWidgetItem *item, int/*column*/) {
	RB_DEBUG->print("DB_ConnectionWidget::on_tree_itemClicked()");
	
    if (!item) {
        return;
    }
    if (!item->parent()) {
        setActive(item);
    } else {
        setActive(item->parent());
        emit tableClicked(item->text(0));
    }
}
