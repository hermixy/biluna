/*****************************************************************
 * $Id: acc_suppliermasterwidget.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Feb 10, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_suppliermasterwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelation.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_SupplierMasterWidget::ACC_SupplierMasterWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
ACC_SupplierMasterWidget::~ACC_SupplierMasterWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel; // shared model
    RB_DEBUG->print("ACC_SupplierMasterWidget::~ACC_SupplierMasterWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_SupplierMasterWidget::init() {

    //
    // 1. Set model
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());
    mModel->setWhere("srm_type_id = 1");

    //
    // 2. Relations, because of separate dockwidget and mdiwidget
    //    mModel->setRelation(mModel->fieldIndex(field),
    //           RB_SqlRelation(relationTable, relationId, relationField));
    mModel->setRelation(mModel->fieldIndex("currency_id"),
           RB_SqlRelation("ACC_Currency", "id", "currcode"));
    mModel->setRelation(mModel->fieldIndex("paymentterm_id"),
           RB_SqlRelation("ACC_PaymentTerm", "id", "terms"));
    mModel->setRelation(mModel->fieldIndex("taxgroup_id"),
           RB_SqlRelation("ACC_TaxGroup", "id", "taxgroupdescr"));
    mModel->setRelation(mModel->fieldIndex("factorcompany_id"),
           RB_SqlRelation("ACC_FactorCompany", "id", "coyname"));
    mModel->setRelation(mModel->fieldIndex("area_id"),
           RB_SqlRelation("ACC_Area", "id", "areacode"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvSupplier, mModel);
    readSettings();

    int count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvSupplier->hideColumn(i);
        } else {
            tvSupplier->showColumn(i);

            if (tvSupplier->columnWidth(i) < 5) {
                tvSupplier->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data to disk
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_SupplierMasterWidget::fileSave(bool withSelect) {
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
void ACC_SupplierMasterWidget::on_pbAdd_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    // Make sure corresponding detail MDI window is open for maybeSave()
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetSupplierDetail);
    mdiWin->show();
    ACC_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("suppliercode"));
    mModel->setData(index, tr("<NEW>"), Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("currency_id"));
    mModel->setData(index, "0", Qt::EditRole);
    QDateTime date = QDateTime::currentDateTime();
    index = mModel->index(row, mModel->fieldIndex("suppliersince"));
    mModel->setData(index, date, Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("paymentterm_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("remittance"));
    mModel->setData(index, 0, Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("taxgroup_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("factorcompany_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("area_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("srm_type_id"));
    mModel->setData(index, 1, Qt::EditRole); // 1 = yes existing ACC supplier
    index = mModel->index(row, mModel->fieldIndex("db_systemuser_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvSupplier->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvSupplier->scrollTo(tvSupplier->currentIndex());
    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Button delete (customer) clicked
 */
void ACC_SupplierMasterWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvSupplier->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button save clicked
 */
void ACC_SupplierMasterWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Set source model filter
 */
void ACC_SupplierMasterWidget::on_pbFilterOn_clicked() {
    filterSourceModel(mModel, "acc_supplier", "suppliercode", leFilter->text());
}

/**
 * Remove source model filter
 */
void ACC_SupplierMasterWidget::on_pbFilterOff_clicked() {
    leFilter->setText("");
    removeFilterSourceModel(mModel);
}

bool ACC_SupplierMasterWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return true;
//    }
//    return false;
}

/**
 * @returns current selected model index
 */
QModelIndex ACC_SupplierMasterWidget::getCurrentModelIndex(int modelType) const {
    if (modelType == ACC_ModelFactory::ModelSupplier) {
        return tvSupplier->currentIndex();
    }
    return QModelIndex();
}

/**
 * Change event, for example translation
 */
void ACC_SupplierMasterWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
