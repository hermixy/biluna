/*****************************************************************
 * $Id: acc_purchaseglpostingdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 28, 2013 17:34:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchaseglpostingdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_qachartmaster.h"
#include "acc_selectchartmasterdialog.h"
#include "acc_selectstockcategorydialog.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_PurchaseGlPostingDialog::ACC_PurchaseGlPostingDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_PurchaseGlPostingDialog::~ACC_PurchaseGlPostingDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_PurchaseGlPostingDialog::~ACC_PurchaseGlPostingDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_PurchaseGlPostingDialog::init() {

    setWindowTitle(tr("Edit Purchase GL Postings[*]"));

    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseGlPosting);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    mMapper = mModel->getMapper();
    addComboBoxMapping(mModel,"taxgroup_id", "ACC_TaxGroup", "id",
                       "taxgroupdescr", cbTaxGroup, mMapper);
    mMapper->addMapping(ileStockCategory,
                        mModel->fieldIndex("stkcat_idx"));
    mMapper->addMapping(ilePurchaseGlAccount,
                        mModel->fieldIndex("purchaseglact_idx"));

    mModel->select();

    setFormatTableView(tvPostings, mModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvPostings->hideColumn(i);
        } else {
            tvPostings->showColumn(i);

            if (tvPostings->columnWidth(i) < 5) {
                tvPostings->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_PurchaseGlPostingDialog::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mModel) {
        if (!mModel->submitAllAndSelect()) {
            QApplication::restoreOverrideCursor();
            return false;
        }
    } else if (mModel) {
        if (!mModel->submitAll()) {
            QApplication::restoreOverrideCursor();
            return false;
        }
    }
    setWindowModified(false);
    ACC_QACHARTMASTER->refresh();
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void ACC_PurchaseGlPostingDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add factor company clicked
 */
void ACC_PurchaseGlPostingDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("taxgroup_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("stkcat_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("purchaseglact_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvPostings->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvPostings->scrollTo(tvPostings->currentIndex());
    cbTaxGroup->setFocus();
}

/**
 * Button delete factor company clicked
 */
void ACC_PurchaseGlPostingDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvPostings->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button stock category clicked
 */
void ACC_PurchaseGlPostingDialog::on_ileStockCategory_clicked() {
    ACC_SelectStockCategoryDialog* dlg = new ACC_SelectStockCategoryDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(
                        index.row(), mModel->fieldIndex("stkcat_idx"));
            mModel->setData(idx, obj->getId()
                            + obj->getValue("categorycode").toString());
        }
    }

    dlg->deleteLater();
}

/**
 * Button purchase GL account clicked
 */
void ACC_PurchaseGlPostingDialog::on_ilePurchaseGlAccount_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(
                        index.row(), mModel->fieldIndex("purchaseglact_idx"));
            mModel->setData(idx, obj->getId()
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString());
        }
    }

    dlg->deleteLater();
}

/**
 * Button help clicked
 */
void ACC_PurchaseGlPostingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_PurchaseGlPostingDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_PurchaseGlPostingDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_PurchaseGlPostingDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
