/*****************************************************************
 * $Id: db_itemdelegate.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 15, 2013 7:26:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ITEMDELEGATE_H
#define DB_ITEMDELEGATE_H

#include <QtWidgets>
#include "db_global.h"


/**
 * General delegate that allows the user to set the relevant column
 * and the required delegate: date, double, integer, line, time.
 * Model to return in RB2::RoleEditor the relevant editor enumerator
 * from RB2::EditorRole
 */
class DB_EXPORT DB_ItemDelegate : public QStyledItemDelegate {

   Q_OBJECT
   
public:
    DB_ItemDelegate(QObject* parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const;

signals:
    // TODO: required?
    void dataChanged(int, int, QVariant&, QVariant&) const;

private:

};

#endif // DB_ITEMDELEGATE_H
