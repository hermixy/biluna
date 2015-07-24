/*****************************************************************
 * $Id: acc_finamtdelegate.cpp 1498 2011-12-22 15:52:46Z rutger $
 * Created: Dec 21, 2011
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_finamtdelegate.h"

#include <QtGui>


/**
 * Constructor
 */
ACC_FinAmtDelegate::ACC_FinAmtDelegate(QObject* parent) : QStyledItemDelegate(parent) {
//    mValidator = NULL;
}

ACC_FinAmtDelegate::~ACC_FinAmtDelegate() {
    // nothing
}


QWidget* ACC_FinAmtDelegate::createEditor(QWidget* parent,
                                          const QStyleOptionViewItem& /*option*/,
                                          const QModelIndex& /*index*/) const {
    QWidget* w = QItemDelegate::createEditor(parent, option, index);
    QLineEdit* le = qobject_cast<QLineEdit*>(w);
    if (!le) {
        le = new QLineEdit(parent);
    }
    le->installEventFilter(const_cast<ACC_FinAmtDelegate*>(this));
    le->setAlignment(Qt::AlignRight);


//    if (index.data(Qt::TextAlignmentRole) == Qt::AlignRight) {
//        le->setAlignment(Qt::AlignRight);
//    }

//    if (mValidator) {
//        // le->setValidator(new QDoubleValidator(0.0, 999.0, 2, this)); example
//        le->setValidator(mValidator);
//    }

    return le;
}


void ACC_FinAmtDelegate::paint(QPainter* painter,
                               const QStyleOptionViewItem& option,
                               const QModelIndex& index) const {
    double amount = index.model()->data(index, Qt::DisplayRole).toDouble();
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight;

    QString str = QString::number(amount, 'f', 2);

    QLineEdit le;


//    drawDisplay(painter, myOption, myOption.rect, str);
//    drawFocus(painter, myOption, myOption.rect);
}

void ACC_FinAmtDelegate::setEditorData(QWidget* editor,
                                       const QModelIndex& /*index*/) const {
//    QVariant value = index.model()->data(index, Qt::DisplayRole);
//    QString str = QString::number(value.toDouble(), 'f', 2);
    QLineEdit* le = static_cast<QLineEdit*>(editor);
//    le->setText(str);
    le->setAlignment(Qt::AlignRight);
}

/*
void ACC_FinAmtDelegate::setModelData(QWidget* editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const {
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    model->setData(index, le->text());
}


void ACC_FinAmtDelegate::updateEditorGeometry(QWidget* editor,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void ACC_FinAmtDelegate::setValidator(QValidator *val) {
    mValidator = val;
}
*/
