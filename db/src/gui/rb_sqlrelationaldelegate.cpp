/*****************************************************************
 * $Id: rb_sqlrelationaldelegate.cpp 2185 2014-10-09 15:09:35Z rutger $
 * Created: Sep 18, 2009 4:57:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor. Set parameter sfpm to NULL if not
 * model is not a proxy model
 * @param sfpm sort filter proxy model
 * @param parent parent object such as a view
 */
RB_SqlRelationalDelegate::RB_SqlRelationalDelegate(RB_MmProxy* sfpm, QObject* parent)
        : QSqlRelationalDelegate(parent) {
    mProxyModel = sfpm;
}

QWidget* RB_SqlRelationalDelegate::createEditor(QWidget* parent,
                                               const QStyleOptionViewItem& option,
                                               const QModelIndex& index) const {
    // TODO: _idx editor

    QModelIndex sourceIndex = mProxyModel->mapToSource(index);

    const RB_MmSource* sqlModel = qobject_cast<const RB_MmSource*>(sourceIndex.model());
    QSqlTableModel* childModel = sqlModel ? sqlModel->relationModel(sourceIndex.column()) : 0;

    if (!childModel) {
        QLineEdit* le = new QLineEdit(parent);
        le->installEventFilter(const_cast<RB_SqlRelationalDelegate*>(this));

        // if (mValidator) {
        //     // le->setValidator(new QDoubleValidator(0.0, 999.0, 2, this)); example
        //     le->setValidator(mValidator);
        // }

        return le;

        // return QItemDelegate::createEditor(parent, option, sourceIndex);
    }

    QComboBox* combo = new QComboBox(parent);
    combo->setModel(childModel);
    combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(sourceIndex.column()).displayColumn()));
    combo->installEventFilter(const_cast<RB_SqlRelationalDelegate *>(this));

    return combo;

}

void RB_SqlRelationalDelegate::setEditorData(QWidget* editor,
                                             const QModelIndex& index) const {
    if (!index.isValid()) {
        return;
    }

    QLineEdit* le = dynamic_cast<QLineEdit*>(editor);

    if (le) {
        QVariant value = index.model()->data(index, Qt::DisplayRole);
        le->setText(value.toString());
        return;
    }

    // idxLineEdit is the lineEdit in RB_IdxLineEdit
    if (editor->objectName() == "idxLineEdit" /*iter != mColumnList.end()*/) {
        QLineEdit* le = dynamic_cast<QLineEdit*>(editor);

        if (le) {
            // remove the Uuid part including the curly braces
            RB_String str = index.data(Qt::DisplayRole).toString();
            le->setText(str);
            return;
        }
    }

    // Relational QComboBox
    QModelIndex sourceIndex = mProxyModel->mapToSource(index);

    const RB_MmSource* sqlModel = qobject_cast<const RB_MmSource*>(sourceIndex.model());
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if (sqlModel && combo) {
        // HACK: for Mac OS X
        QVariant v = index.data(Qt::DisplayRole);
        QByteArray n = combo->metaObject()->userProperty().name();

        if (!n.isEmpty()) {
            if (!v.isValid())
                v = QVariant(combo->property(n).userType(), (const void *)0);
            combo->setProperty(n, v);
        }
        // Was originally only next line:
        // combo->setCurrentIndex(combo->findText(sqlModel->data(sourceIndex).toString()));

        return;
    }

    // Image QLabel
    QLabel* label = qobject_cast<QLabel*>(editor);
    if (label) {
        QByteArray imageData = index.data(Qt::EditRole).toByteArray();
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            label->setPixmap(pixmap);
        } else {
            label->setPixmap(QPixmap());
        }
    }

    QItemDelegate::setEditorData(editor, sourceIndex);
    return;
}

void RB_SqlRelationalDelegate::setModelData(QWidget* editor,
                                            QAbstractItemModel* model,
                                            const QModelIndex& index) const {
    QLineEdit* le = dynamic_cast<QLineEdit*>(editor);

    if (le) {
        model->setData(index, le->text());
        return;
    }

    // RB_IdxLineEdit, _idx field with Uuid and text
//    int col = index.column();
//    std::set<int>::iterator iter = mColumnList.find(col);

    // idxLineEdit is the lineEdit in RB_IdxLineEdit
    if (editor->objectName() == "idxLineEdit" /*iter != mColumnList.end()*/) {
        QLineEdit* le = dynamic_cast<QLineEdit*>(editor);

        if (le) {
            // The lineEdit does not has the relevant
            // Uuid part including the curly braces and display value
            // Set data is done directly to model.
            // QItemDelegate::setModelData(editor, model, index);
            return;
        }
    }

    // QComboBox and others
    QModelIndex sourceIndex = mProxyModel->mapToSource(index);

    if (!index.isValid() || !sourceIndex.isValid())
        return;

    RB_MmSource* sqlModel = qobject_cast<RB_MmSource*>(mProxyModel->sourceModel());
    QSqlTableModel* childModel = sqlModel ? sqlModel->relationModel(sourceIndex.column()) : 0;

    QComboBox *combo = qobject_cast<QComboBox *>(editor);
    if (!sqlModel || !childModel || !combo) {
        QItemDelegate::setModelData(editor, model, index); // lineEdits, etc.
        return;
    }

    int currentItem = combo->currentIndex();
    int childColIndex = childModel->fieldIndex(sqlModel->relation(sourceIndex.column()).displayColumn());
    int childEditIndex = childModel->fieldIndex(sqlModel->relation(sourceIndex.column()).indexColumn());
    sqlModel->setData(sourceIndex,
            childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole),
            Qt::DisplayRole);
    sqlModel->setData(sourceIndex,
            childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole),
            Qt::EditRole);

}

void RB_SqlRelationalDelegate::setIdxColumn(int col) {
    mColumnList.insert(col);
}

void RB_SqlRelationalDelegate::clearIdxColumnList() {
    mColumnList.clear();
}
