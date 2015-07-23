/*****************************************************************
 * $Id: rb_cmbdelegate.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 18, 2010 9:25:40 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_CMBDELEGATE_H
#define RB_CMBDELEGATE_H

#include <QtWidgets>
#include "db_global.h"

class RB_MmProxy;

/**
 * Basic combobox delegate for model views
 */
class DB_EXPORT RB_CmbDelegate : public QStyledItemDelegate {

    Q_OBJECT

public:
    RB_CmbDelegate(QObject* parent, const QStringList& strL,
                   RB_MmProxy* proxyModel = NULL);
    virtual ~RB_CmbDelegate();

    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
//    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
//                       const QModelIndex& index ) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual QString at(int index) const;

private:
    void paintWidget(QPainter *painter, const QRect &rect,
                     const QString &cacheKey, QWidget *widget) const;

    QStringList mStrL;
    QLabel* mLabel;
    RB_MmProxy* mProxyModel;

};

#endif // RB_CMBDELEGATE_H
