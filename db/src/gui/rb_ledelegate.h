/*****************************************************************
 * $Id: rb_ledelegate.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_LEDELEGATE_H
#define RB_LEDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include "db_global.h"

class QValidator;

/**
 * Basic line edit delegate for model views
 */
class DB_EXPORT RB_LeDelegate : public QStyledItemDelegate {
	
    Q_OBJECT

public:
    RB_LeDelegate(QObject* parent = 0);
	virtual ~RB_LeDelegate();

    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const;
    virtual void setValidator(QValidator* val);

protected:
    QValidator* mValidator;

};

#endif /*RB_LEDELEGATE_H*/
