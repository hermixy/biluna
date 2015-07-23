/*****************************************************************
 * $Id: rb_datawidgetmapper.cpp 2144 2014-05-01 18:22:24Z rutger $
 * Created: Sep 1, 2009 4:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_datawidgetmapper.h"
#include <QtWidgets>

#include "db_dialogfactory.h"
#include "rb_idxlineedit.h"
// #include "rb_idxlineeditdelegate.h"
#include "rb_objectbase.h"
#include "rb_mmproxy.h"
#include "rb_sqlrelationaldelegate.h"


/**
 * Constructor
 */
RB_DataWidgetMapper::RB_DataWidgetMapper(RB_MmProxy* sfpm)
        : QDataWidgetMapper(sfpm) {
    mProxyModel = sfpm;
    setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    setItemDelegate(new RB_SqlRelationalDelegate(sfpm, this));

    connect(this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotActivateWidgets(int)));
}

/**
 * Destructor
 */
RB_DataWidgetMapper::~RB_DataWidgetMapper() {
    mObjectList.clear();
}

/**
 * Sets the root item to index. This can be used to display a branch of a tree.
 * Pass an invalid model index to display the top-most branch. Index will be
 * mapped to source model in case of a proxy model implementation.
 *@param index (current) root index
 */
void RB_DataWidgetMapper::setRootIndex(const QModelIndex &index) {
    QDataWidgetMapper::setRootIndex(index);
}

/**
 * Add widget mapping to section.
 * TODO: allow for column position change
 * @param widget widget to be mapped to section (column of data model)
 * @param section number of data model
 */
void RB_DataWidgetMapper::addMapping(QWidget* widget, int section) {
#ifdef Q_OS_MAC
    // HACK: extra for the MAC otherwise the model does not get updated
    QComboBox* cmb = dynamic_cast<QComboBox*>(widget);

    if (cmb) {
        connect(cmb, SIGNAL(activated(int)),
                this, SLOT(slotRelationComboIndexChanged(int)));
    }
#endif

    QWidget* wdgt = widget;
    RB_IdxLineEdit* ile = dynamic_cast<RB_IdxLineEdit*>(widget);
    if (ile) {
        wdgt = ile->getLineEdit();
        ile->setSection(section);
        ile->setModel(mProxyModel);
    }

    QDataWidgetMapper::addMapping(wdgt, section);
    wdgt->installEventFilter(this);
    connect(wdgt, SIGNAL(destroyed(QObject*)),
            this, SLOT(slotRemoveMapping(QObject*)));

    // Add widget to mapped list
    mObjectList.append(widget);
    widget->setEnabled(false);
}

/**
 * Add widget mapping to section.
 * TODO: allow for column position change
 * @param widget widget to be mapped to section (column of data model)
 * @param section field number of data model
 * @param propertyName name of widget property to be mapped
 */
void RB_DataWidgetMapper::addMapping(QWidget* widget, int section,
                                     const QByteArray& propertyName)  {
    QComboBox* cmb = dynamic_cast<QComboBox*>(widget);

    if (cmb && propertyName == RB_String("currentIndex")) {
        // HACK: combobox with fixed items and integers in database
        // currentText does not work.
        // TODO: Will emit currentChanged twice
        // Map combobox to integer value in database.
        // The tableView or treeView shows also the text because a text list
        // has been set in the model to convert the integer to text in the
        // tableView. The 'currentText' property does not work for mapping.
        // In this function the combobox is connected via signal/slot
        // to the original integer in the database setting the combobox
        // currentIndex.
        mComboBoxMap[cmb] = section;

#ifdef Q_OS_MAC
        // HACK: extra for the MAC otherwise the model does not get updated
        connect(cmb, SIGNAL(activated(int)),
                this, SLOT(slotFixedComboIndexChanged(int)));

#endif
    }

    QWidget* wdgt = widget;
    RB_IdxLineEdit* ile = dynamic_cast<RB_IdxLineEdit*>(widget);

    if (ile) {
        // get lineEdit from Idx field
        wdgt = ile->getLineEdit();
        ile->setSection(section);
        ile->setModel(mProxyModel);
    }

    QDataWidgetMapper::addMapping(wdgt, section, propertyName);
    wdgt->installEventFilter(this);
    connect(wdgt, SIGNAL(destroyed(QObject*)),
            this, SLOT(slotRemoveMapping(QObject*)));

    // Add widget to mapped list
    mObjectList.append(widget);
    widget->setEnabled(false);
}

/**
 * Remove widget from mapper.
 * @param widget to be removed from mapping
 */
void RB_DataWidgetMapper::removeMapping(QWidget* widget) {
    QWidget* wdgt = widget;
    RB_IdxLineEdit* ile = dynamic_cast<RB_IdxLineEdit*>(widget);
    if (ile) wdgt = ile->getLineEdit();

    QDataWidgetMapper::removeMapping(wdgt);
    mObjectList.remove(mObjectList.indexOf(widget));
}

/**
 * Clear all mappings
 */
void RB_DataWidgetMapper::clearMapping() {
    QDataWidgetMapper::clearMapping();
    mObjectList.clear();
}

/**
 * Slot called when widget is deleted, removes widget from mapping
 * @param object
 */
void RB_DataWidgetMapper::slotRemoveMapping(QObject* obj) {
    if (obj) {
        mObjectList.remove(mObjectList.indexOf(obj));
    }
}

/**
 * Slot is called after model changes have been submitted,
 * this gave problems with larger amount of models in setModelIndex()
 */
void RB_DataWidgetMapper::slotClearWidgets() {
    for (int i = 0; i < mObjectList.size(); ++i) {
        QObject* obj = mObjectList.at(i);
        QCheckBox* chb = dynamic_cast<QCheckBox*>(obj);
        QComboBox* cmb = dynamic_cast<QComboBox*>(obj);
        QDateEdit* de = dynamic_cast<QDateEdit*>(obj);
        QDateTimeEdit* dte = dynamic_cast<QDateTimeEdit*>(obj);
        QDoubleSpinBox* dsb = dynamic_cast<QDoubleSpinBox*>(obj);
        QLabel* lbl = dynamic_cast<QLabel*>(obj);
        QLineEdit* le = dynamic_cast<QLineEdit*>(obj);
        QPlainTextEdit* pte = dynamic_cast<QPlainTextEdit*>(obj);
        QSpinBox* sb = dynamic_cast<QSpinBox*>(obj);
        QTextEdit* te = dynamic_cast<QTextEdit*>(obj);
        RB_IdxLineEdit* ile = dynamic_cast<RB_IdxLineEdit*>(obj);

        if (chb) {
            chb->setEnabled(false);
            chb->setChecked(false);
        } else if (cmb) {
            cmb->setEnabled(false);
            cmb->setCurrentIndex(0);
        } else if (de) {
            de->setEnabled(false);
            de->clear();
        } else if (dte) {
            dte->setEnabled(false);
            dte->clear();
        } else if (dsb) {
            dsb->setEnabled(false);
            dsb->setValue(0.0);
        } else if (lbl) {
            // image label
            lbl->setPixmap(QPixmap()); // clear();
        } else if (le) {
            le->setEnabled(false);
            le->clear(); // setText(""); // ->clear();
        } else if (pte) {
            pte->setEnabled(false);
            pte->clear();
        } else if (sb) {
            sb->setEnabled(false);
            sb->setValue(0);
        } else if (te) {
            te->setEnabled(false);
            te->clear();
        } else if (ile) {
            ile->setEnabled(false);
            le = ile->getLineEdit();
            le->clear(); // setText(""); // ->clear();
        }
    }

}

/**
 * Set current model index. Override required for tree model otherwise
 * setRootIndex(idx.parent()) is not done
 * @param index current model index
 */
void RB_DataWidgetMapper::setCurrentModelIndex(const QModelIndex& idx) {

    if (!idx.isValid()) {
        QDataWidgetMapper::setRootIndex(QModelIndex());
        QDataWidgetMapper::setCurrentModelIndex(QModelIndex());
        slotClearWidgets();
    } else {
        QDataWidgetMapper::setRootIndex(idx.parent());
        QDataWidgetMapper::setCurrentModelIndex(idx);

        // HACK: currentText does not work
        if (mComboBoxMap.size() > 0) {
            int row = idx.row();
            QMapIterator<QComboBox*, int> iter(mComboBoxMap);

            while (iter.hasNext()) {
                iter.next();

                int col = iter.value();
                QVariant var = mProxyModel->data(
                            mProxyModel->index(row, col, idx.parent()),
                            RB2::RoleOrigData);

                QComboBox* cmb = iter.key();
                cmb->setCurrentIndex(var.toInt());
            }
        }
    }
}

void RB_DataWidgetMapper::toPrevious() {
    QDataWidgetMapper::toPrevious();
    updateSelectionModel(currentIndex());
}

void RB_DataWidgetMapper::toNext() {
    QDataWidgetMapper::toNext();
    updateSelectionModel(currentIndex());
}

/**
 * Slot to activate mapped widgets. Activation includes enabling and
 * setting the style according the style data
 * TODO: this slot is connected to the signal currentIndexChanged(), this
 * index can be the row but also the column. The column is not included yet.
 * @param row current row
 */
void RB_DataWidgetMapper::slotActivateWidgets(int row) {
    if (row < 0 || row >= mProxyModel->rowCount()) {
        return;
    }

    QModelIndex parent = this->rootIndex();

    // change background color if applicable
    QWidget* w = NULL;
    int section = -1;
    QVariant var = QVariant();

    for (int i = 0; i < mObjectList.size(); ++i) {
        w = dynamic_cast<QWidget*>(mObjectList.at(i));
        section = mappedSection(w);
        var = mProxyModel->data(
                mProxyModel->index(row, section, parent),
                Qt::UserRole);
        QString str = var.toString();

        if (!str.isEmpty()) {
            RB_IdxLineEdit* ile = dynamic_cast<RB_IdxLineEdit*>(w);
            if (ile) {
                ile->getLineEdit()->setStyleSheet(str);
            } else {
                w->setStyleSheet(str);
            }
        }

        w->setEnabled(true); // could have been done with signal/slot

        // HACK:
        RB_IdxLineEdit* ile = dynamic_cast<RB_IdxLineEdit*>(w);
        if (ile) {
            ile->setEnabled(true);
        }
    }
}

#ifdef Q_OS_MAC
/**
 * HACK: extra for the MAC otherwise the model does not get updated
 * @brief RB_DataWidgetMapper::slotFixedComboIndexChanged
 * @param index
 */
void RB_DataWidgetMapper::slotFixedComboIndexChanged(int /*index*/) {
    submit();
    model()->submit();
}

/**
 * HACK: extra for the MAC otherwise the model does not get updated
 * @brief RB_DataWidgetMapper::slotRelationComboIndexChanged
 * @param index
 */
void RB_DataWidgetMapper::slotRelationComboIndexChanged(int /*index*/) {
    submit();
    model()->submit();
}
#endif

/**
 * Update selection model after mapper has changed row
 */
void RB_DataWidgetMapper::updateSelectionModel(int row) {
    QItemSelectionModel* selModel = mProxyModel->getSelectionModel();

    if (!selModel) {
        return;
    }

    QModelIndex prevIdx = selModel->currentIndex();
    int col = RB2::HIDDENCOLUMNS;

    if (prevIdx.isValid()) {
        col = prevIdx.column();
    }

    QModelIndex parent = prevIdx.parent();
    QModelIndex curIdx = mProxyModel->index(row, col, parent);
    selModel->setCurrentIndex(curIdx, QItemSelectionModel::SelectCurrent);
}

/**
 * Set proxy model
 * @param sfpm sort filter proxy model
 */
void RB_DataWidgetMapper::setProxyModel(RB_MmProxy* sfpm) {
    mProxyModel = sfpm;
}

