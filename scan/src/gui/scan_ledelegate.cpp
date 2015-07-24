/*****************************************************************
 * $Id: scan_ledelegate.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: 2011-04-28 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_ledelegate.h"


#include <QtWidgets>
#include "rb_mmproxy.h"


/**
 * Constructor
 */
SCAN_LeDelegate::SCAN_LeDelegate(QObject* parent) : QItemDelegate(parent) {
    // nothing
}

/**
 * Destructor
 */
SCAN_LeDelegate::~SCAN_LeDelegate() {
	// nothing
}

QWidget* SCAN_LeDelegate::createEditor(QWidget* parent,
                                    const QStyleOptionViewItem& /*option*/,
                                    const QModelIndex& index) const {
    if (!index.isValid()) return NULL;
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    if (index.column() != m->fieldIndex("mvalue")) return NULL;

    QLineEdit* editor = new QLineEdit(parent);
    editor->installEventFilter(const_cast<SCAN_LeDelegate*>(this));
    return editor;
}


void SCAN_LeDelegate::setEditorData(QWidget* editor,
                                    const QModelIndex& index) const {
    if (!index.isValid()) return;
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    if (index.column() != m->fieldIndex("mvalue")) return;

    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value.toString());
}


void SCAN_LeDelegate::setModelData(QWidget* editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    if (!index.isValid()) return;
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    if (index.column() != m->fieldIndex("mvalue")) return;

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	model->setData(index, lineEdit->text());
}


void SCAN_LeDelegate::updateEditorGeometry(QWidget* editor,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const {
    if (!index.isValid()) return;
    const RB_MmProxy* m = dynamic_cast<const RB_MmProxy*>(index.model());
    if (index.column() != m->fieldIndex("mvalue")) return;

    editor->setGeometry(option.rect);
}
