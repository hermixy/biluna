/*****************************************************************
 * $Id: rb_mapperdelegate.h 2248 2016-04-12 09:13:00Z rutger $
 * Created: Apr 12, 2016 9:25:40 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MAPPERDELEGATE_H
#define RB_MAPPERDELEGATE_H

#include <QtWidgets>
#include "db_global.h"

//class RB_MmProxy;

/**
 * Base delegate for RB_DataWidgetMapper to show doubles in engineering or
 * scientific notation or financial notation
 */
class DB_EXPORT RB_MapperDelegate : public QStyledItemDelegate {

    Q_OBJECT

public:
    RB_MapperDelegate(QObject* parent);
    virtual ~RB_MapperDelegate();

//    virtual QWidget* createEditor(QWidget *parent,
//                                  const QStyleOptionViewItem &option,
//                                  const QModelIndex &index) const;
//    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
//                       const QModelIndex& index ) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex&index) const;
//    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
//                              const QModelIndex &index) const;

//    virtual void updateEditorGeometry(QWidget *editor,
//        const QStyleOptionViewItem &option, const QModelIndex &index) const;

//    virtual QString at(int index) const;

private:
//    void paintWidget(QPainter *painter, const QRect &rect,
//                     const QString &cacheKey, QWidget *widget) const;

//    QStringList mStrL;
//    QLabel* mLabel;
//    RB_MmProxy* mProxyModel;
//    bool mIsSetText;

};

#endif // RB_MAPPERDELEGATE_H
