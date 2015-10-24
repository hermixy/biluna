/*****************************************************************
 * $Id: rb_ledelegate.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_memberdelegate.h"

#include <QDoubleValidator>
#include <QIntValidator>
#include <QtWidgets>
#include "rb_objectbase.h"


/**
 * The QLineEdit delegate that allows the user to change text
 * and number values from model object members using different validators
 */
RB_MemberDelegate::RB_MemberDelegate(QObject* parent)
            : QStyledItemDelegate(parent) {
    mObject = NULL;
}

RB_MemberDelegate::~RB_MemberDelegate() {
    // nothing
}

QWidget* RB_MemberDelegate::createEditor(QWidget* parent,
                                         const QStyleOptionViewItem& /*option*/,
                                         const QModelIndex& index) const {
    QLineEdit* le = new QLineEdit(parent);
    le->installEventFilter(const_cast<RB_MemberDelegate*>(this));

    if (!mObject) {
        return le;
    }

    RB_ObjectMember* mem = mObject->getMember(index.row());

    if (!mem) {
        return le;
    }

    if (mem->getType() == RB2::MemberDouble) {
        le->setValidator(new QDoubleValidator(le));
    } else if (mem->getType() == RB2::MemberInteger) {
        le->setValidator(new QIntValidator(le));
    }

    return le;
}


void RB_MemberDelegate::setEditorData(QWidget* editor,
                                      const QModelIndex& index) const {
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    le->setText(value.toString());
}


void RB_MemberDelegate::setModelData(QWidget* editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const {
    QLineEdit* le = static_cast<QLineEdit*>(editor);
    model->setData(index, le->text());
}


void RB_MemberDelegate::updateEditorGeometry(QWidget* editor,
                                             const QStyleOptionViewItem& option,
                                             const QModelIndex& /*index*/) const {
    editor->setGeometry(option.rect);
}

void RB_MemberDelegate::setObject(RB_ObjectBase* obj) {
    mObject = obj;
}
