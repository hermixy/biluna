/*****************************************************************
 * $Id: acc_finamtdelegate.h 1498 2011-12-22 15:52:46Z rutger $
 * Created: Dec 21, 2011
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_FINAMTDELEGATE_H
#define ACC_FINAMTDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>

//class QValidator;

/**
 * Delegate to show double number as financial amounts
 */
class ACC_FinAmtDelegate : public QStyledItemDelegate {
	
    Q_OBJECT

public:
    ACC_FinAmtDelegate(QObject* parent = 0);
    virtual ~ACC_FinAmtDelegate();

    virtual QWidget* createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
//    virtual bool editorEvent(QEvent* event, QAbstractItemModel* model,
//                             const QStyleOptionViewItem& option, const QModelIndex& index);
//    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
//                       const QModelIndex& index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
//                              const QModelIndex &index) const;
//    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
//                                      const QModelIndex &index) const;
//    virtual void setValidator(QValidator* val);

protected:
//    QValidator* mValidator;

};

#endif /*ACC_FINAMTDELEGATE_H*/
