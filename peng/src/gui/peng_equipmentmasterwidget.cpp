/*****************************************************************
 * $Id: peng_equipmentmasterwidget.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna peng project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_equipmentmasterwidget.h"

#include <QStringListModel>
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
PENG_EquipmentMasterWidget::PENG_EquipmentMasterWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
PENG_EquipmentMasterWidget::~PENG_EquipmentMasterWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel; // shared model
    RB_DEBUG->print("PENG_EquipmentMasterWidget::~PENG_EquipmentMasterWidget() OK");
}

/**
 * Initialize widget and models
 */
void PENG_EquipmentMasterWidget::init() {
    // TODO: implement filter
    this->cbEquipmentType->hide();

    // Set model and connect to table view
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipment);
    mModel->setRoot(PENG_MODELFACTORY->getRootId());

    // Relations, because of separate dockwidget and mdiwidget
    // mModel->setRelation(mModel->fieldIndex(field),
    //        QSqlRelation(relationTable, relationId, relationField));

    // Set standard format and connection for model and view
    setFormatTableView(tvEquipment, mModel);

    // Hide columns
    for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS) {
            tvEquipment->hideColumn(i);
        }
    }

    mModel->select();
    readSettings();

    // debug only
//    QString sql = mModel->sourceModel()->query().lastQuery();
    //    RB_DEBUG->print(sql);
}


/**
 * Save data to disk
 * @param withSelect true if select() is to be done after submitAll()
 */
bool PENG_EquipmentMasterWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add (customer) clicked
 */
void PENG_EquipmentMasterWidget::on_pbAdd_clicked() {
    if (!mModel) return;

    // Make sure corresponding detail MDI window is open for maybeSave()
// TODO:
//    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
//            PENG_DialogFactory::WidgetEquipmentDetail);
//    mdiWin->show();
//    PENG_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    if (mModel->isModelModified()) {
        fileSave(true);
    }

    // Clear sorting and filtering of proxyModel
    mModel->undoFilterSort();
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("number"));
    mModel->setData(index, "<NEW>", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("type_idx"));
    mModel->setData(index, "0", Qt::EditRole);

    tvEquipment->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvEquipment->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
}

/**
 * Button delete (customer) clicked
 */
void PENG_EquipmentMasterWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = PENG_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvEquipment->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);

        if (row > 0) {
            tvEquipment->setCurrentIndex(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tvEquipment->scrollTo(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

/**
 * Button save clicked
 */
void PENG_EquipmentMasterWidget::on_pbSave_clicked() {
    fileSave(true);
}

bool PENG_EquipmentMasterWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return RB_Widget::closeWidget();
//    }
//    return false;
}

/**
 * Change event, for example translation
 */
void PENG_EquipmentMasterWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
