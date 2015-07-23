/*****************************************************************
 * $Id: rb_ledelegate.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_ledelegate.h"

#include <QtWidgets>


/**
 * The basic delegate that allows the user to change text and number values 
 * from the model using QLineEdit
 */
RB_LeDelegate::RB_LeDelegate(QObject* parent) : QStyledItemDelegate(parent) {
    mValidator = NULL;
}

RB_LeDelegate::~RB_LeDelegate() {
    // nothing
}

QWidget* RB_LeDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& /*option*/,
                                     const QModelIndex& /*index*/) const {
    QLineEdit* le = new QLineEdit(parent);
    le->installEventFilter(const_cast<RB_LeDelegate*>(this));

    if (mValidator) {
        // le->setValidator(new QDoubleValidator(0.0, 999.0, 2, this)); example
        le->setValidator(mValidator);
    }

    return le;
}


void RB_LeDelegate::setEditorData(QWidget* editor,
                                  const QModelIndex& index) const {
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    le->setText(value.toString());
}


void RB_LeDelegate::setModelData(QWidget* editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const {
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    model->setData(index, le->text());
}


void RB_LeDelegate::updateEditorGeometry(QWidget* editor,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& /*index*/) const {
    editor->setGeometry(option.rect);
}

void RB_LeDelegate::setValidator(QValidator *val) {
    mValidator = val;
}
