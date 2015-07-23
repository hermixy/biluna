/*****************************************************************
 * $Id: db_itemdelegate.cpp 2026 2013-11-05 18:10:48Z rutger $
 * Created: Jan 15, 2013 7:26:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_itemdelegate.h"

#include "rb.h"


/**
 * Constructor
 */
DB_ItemDelegate::DB_ItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {
   // nothing
}

/**
 * Returns the widget used to edit the item specified by index for editing.
 * The parent widget and style option are used to control how
 * the editor widget appears.
 * @param parent parent widget
 * @param option style view options
 * @param index model index
 */
QWidget* DB_ItemDelegate::createEditor(QWidget* parent,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& index) const {
    int editorFormat = index.model()->data(index, RB2::RoleEditor).toInt();
    QWidget* editor;

    if (editorFormat == RB2::EditorLine) {
        editor = new QLineEdit(parent);
    } else if (editorFormat == RB2::EditorDate) {
        QDateEdit* de = new QDateEdit(parent);
        de->setDisplayFormat("yyyy-MM-dd");
        de->setCalendarPopup(true);
        editor = de;
    } else if (editorFormat == RB2::EditorTime) {
        QTimeEdit* te = new QTimeEdit(parent);
        te->setDisplayFormat("h:mm:ss");
        editor = te;
    } else if (editorFormat == RB2::EditorInteger) {
        QSpinBox* sb = new QSpinBox(parent);
        //how to set the range to infinite?
        sb->setRange(-999999999, 999999999);
        editor = sb;
    } else if (editorFormat == RB2::EditorDouble) {
        QDoubleSpinBox* dsb = new QDoubleSpinBox(parent);
        //how to set the range to infinite?
        dsb->setRange(-9999999.9999999, 9999999.9999999);
        dsb->setDecimals(3);
        dsb->setSingleStep(0.1);
        editor = dsb;
    } else { //should not happen
        editor = QStyledItemDelegate::createEditor(parent, option, index);
        return editor;
    }

    editor->installEventFilter(const_cast<DB_ItemDelegate*>(this));
    return editor;
}

/**
 * Set editor data
 * @param editor to be set with data
 * @param index current model index
 */
void DB_ItemDelegate::setEditorData(QWidget* editor,
                                    const QModelIndex& index) const {
    int editorFormat = index.model()->data(index, RB2::RoleEditor).toInt();
    QVariant value = index.model()->data(index, Qt::EditRole);

    if (editorFormat == RB2::EditorLine) {
        QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
        lineEdit->setText(value.toString());
    } else if (editorFormat == RB2::EditorDate) {
        QDateEdit* dateEdit = dynamic_cast<QDateEdit*>(editor);
        dateEdit->setDate(QDate::fromString(value.toString(), "yyyy-MM-dd"));
    } else if (editorFormat == RB2::EditorTime) {
        QTimeEdit* timeEdit = dynamic_cast<QTimeEdit*>(editor);
        timeEdit->setTime(QTime::fromString(value.toString(), "h:mm:ss"));
    } else if (editorFormat == RB2::EditorInteger) {
        QSpinBox* spinBox = dynamic_cast<QSpinBox*>(editor);
        spinBox->setValue(value.toInt());
    } else if (editorFormat == RB2::EditorDouble) {
        QDoubleSpinBox* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
        doubleSpinBox->setValue(value.toDouble());
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

/**
 * Gets data from the editor widget and stores it
 * in the specified model at the item index.
 * @param editor editor widget
 * @param model data model
 * @param index model index
 */
void DB_ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                   const QModelIndex& index) const {
    QVariant oldData = model->data(index, Qt::EditRole);
    QVariant newData;
    int editorFormat = index.model()->data(index, RB2::RoleEditor).toInt();
   
    if (editorFormat == RB2::EditorLine) {
        QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
        newData = lineEdit->text();
    } else if (editorFormat == RB2::EditorDate) {
        QDateEdit* dateEdit = dynamic_cast<QDateEdit*>(editor);
        newData = dateEdit->date().toString(Qt::ISODate);
    } else if (editorFormat == RB2::EditorTime) {
        QTimeEdit* timeEdit = dynamic_cast<QTimeEdit*>(editor);
        newData = timeEdit->time();
    } else if (editorFormat == RB2::EditorInteger) {
        QSpinBox* spinBox = dynamic_cast<QSpinBox*>(editor);
        newData = spinBox->value();
    } else if (editorFormat == RB2::EditorDouble) {
        QDoubleSpinBox* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
        newData = doubleSpinBox->value();
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
        return;
    }

    if (oldData != newData) {
        model->setData(index, newData);
        emit dataChanged(index.row(), index.column(), oldData, newData);
    }
}


