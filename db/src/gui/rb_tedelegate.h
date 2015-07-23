/*****************************************************************
 * $Id: rb_ledelegate.h 782 2008-08-24 17:27:42Z rutger $
 * Created: 2011-05-10 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TEDELEGATE_H
#define RB_TEDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QTextDocument>
#include "db_global.h"

class QLabel;


/**
 * Basic textedit delegate for model views handling richt text (HTML)
 */
class DB_EXPORT RB_TeDelegate : public QStyledItemDelegate {
	
    Q_OBJECT

public:
    RB_TeDelegate(QObject* parent = 0);
        virtual ~RB_TeDelegate();

    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
                       const QModelIndex& index ) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& option,
                           const QModelIndex& index ) const;
    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void paintWidget(QPainter *painter, const QRect &rect,
                     const QString &cacheKey, QWidget *widget) const;

//    int mHeight;
    QLabel* mLabel;
    mutable QTextDocument mDocument;
};

#endif /*RB_TEDELEGATE_H*/
