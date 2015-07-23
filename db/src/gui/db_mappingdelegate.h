/*****************************************************************
 * $Id: db_mappingdelegate.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_MAPPINGDELEGATE_H
#define DB_MAPPINGDELEGATE_H

#include <QItemDelegate>
#include "db_global.h"

class DB_EXPORT DB_MappingDelegate : public QItemDelegate {
	
    Q_OBJECT

public:
    DB_MappingDelegate(QObject *parent = 0);
    virtual ~DB_MappingDelegate() { }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setComboBoxData(const QStringList& columnList);
        
private slots:
    // void emitCommitData();
    
private:
	QStringList comboBoxData;
	
};

#endif // DB_MAPPINGDELEGATE_H
