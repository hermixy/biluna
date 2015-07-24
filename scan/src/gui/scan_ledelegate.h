/*****************************************************************
 * $Id: scan_ledelegate.h 1403 2011-04-28 20:56:30Z rutger $
 * Created: 2011-04-28 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_LEDELEGATE_H
#define SCAN_LEDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>


/**
 * The basic delegate that allows the user to change text and number values
 * from the model using QLineEdit.
 * This delegate is used in SCAN_ScanResultWidget leaving only
 * on column (mvalue) editable
 */
class SCAN_LeDelegate : public QItemDelegate {
	
    Q_OBJECT

public:
    SCAN_LeDelegate(QObject* parent = 0);
    virtual ~SCAN_LeDelegate();

    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*SCAN_LEDELEGATE_H*/
