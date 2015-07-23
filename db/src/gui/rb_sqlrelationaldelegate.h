/*****************************************************************
 * $Id: rb_sqlrelationaldelegate.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 18, 2009 4:57:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SQLRELATIONALDELEGATE_H
#define RB_SQLRELATIONALDELEGATE_H

#include <set>
#include <QSqlRelationalDelegate>
#include <QSortFilterProxyModel>
#include <QtWidgets>
#include "rb_mmproxy.h"

/**
 * Relational delegate for proxy models and extended ID fields (_idx)
 */
class DB_EXPORT RB_SqlRelationalDelegate : public QSqlRelationalDelegate {

public:
    RB_SqlRelationalDelegate(RB_MmProxy* aModel, QObject* parent);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex &index) const;
    void setIdxColumn(int col);
    void clearIdxColumnList();

private:
    RB_MmProxy* mProxyModel;
    std::set<int> mColumnList;

};

#endif // RB_SQLRELATIONALDELEGATE_H
