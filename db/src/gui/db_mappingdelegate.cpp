/*****************************************************************
 * $Id: db_mappingdelegate.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_mappingdelegate.h"

#include <QtWidgets>
#include <iostream>

#include "db_mappingdialog.h"


/**
 * Delegate that allows the user to change the data in the first column of the
 * tableWidget but not the other fields. The data that can be changed is in a
 * comboBox
 */
DB_MappingDelegate::DB_MappingDelegate(QObject *parent) : QItemDelegate(parent) {

}

QWidget* DB_MappingDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& /* option */,
                                     const QModelIndex& index) const {
    if (index.column() == 0) {
    	QComboBox* editor = new QComboBox(parent);
        
//        editor->addItem(tr("Normal"));
//        editor->addItem(tr("Active"));
//        editor->addItem(tr("Disabled"));
		foreach (QString dataItem, comboBoxData) {
			editor->addItem(dataItem);
		}

	    editor->installEventFilter(const_cast<DB_MappingDelegate*>(this));
	    return editor;
    }
    
    QLineEdit* editor = new QLineEdit(parent);
    editor->setEnabled(false);

    return editor;
}

void DB_MappingDelegate::setEditorData(QWidget* editor,
                                  const QModelIndex& index) const {
    // other fields are not editable
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (!comboBox) {
        return;
    }
    int pos = comboBox->findText(index.model()->data(index).toString(),
                                 Qt::MatchExactly);
    comboBox->setCurrentIndex(pos);
}

void DB_MappingDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const {
    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (!comboBox)
        return;

    model->setData(index, comboBox->currentText());
}

void DB_MappingDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const {
    editor->setGeometry(option.rect);
}

void DB_MappingDelegate::setComboBoxData(const QStringList& columnList) {
	comboBoxData = columnList;	
}

//void DB_MappingDelegate::emitCommitData() {
//    emit commitData(qobject_cast<QWidget *>(sender()));
//}
