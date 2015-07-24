/*****************************************************************
 * $Id: acc_salestaxpostingdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 19, 2012 4:25:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salestaxpostingdialog.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_qachartmaster.h"
#include "acc_qasysacctcontrol.h"
#include "acc_selectchartmasterdialog.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_SalesTaxPostingDialog::ACC_SalesTaxPostingDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_SalesTaxPostingDialog::~ACC_SalesTaxPostingDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mMapper; mappers are deleted when model is deleted
    delete mModel; // the unique model is used, therefore delete here
    RB_DEBUG->print("ACC_SalesTaxPostingDialog::~ACC_SalesTaxPostingDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_SalesTaxPostingDialog::init() {

    setWindowTitle(tr("Edit Sales Tax Postings[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    // Shared is false because ChartMaster is unfiltered compared to other
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesTaxPosting, false);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    //
    // Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    addComboBoxMapping(mModel, "taxauth_id", "ACC_TaxAuthority", "id",
                       "description", cbTaxAuthority, mMapper);
    addComboBoxMapping(mModel, "taxcat_id", "ACC_TaxCategory", "id",
                       "taxcatname", cbTaxCategory, mMapper);
    mMapper->addMapping(ileSalesTaxAccount,
                        mModel->fieldIndex("salestaxacct_idx"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvSalesTaxPosting, mModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvSalesTaxPosting->hideColumn(i);
        } else {
            tvSalesTaxPosting->showColumn(i);

            if (tvSalesTaxPosting->columnWidth(i) < 5) {
                tvSalesTaxPosting->setColumnWidth(i, 100);
            }
        }
    }


}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool ACC_SalesTaxPostingDialog::fileSave(bool withSelect) {
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
    return true;
}

/**
 * Reverse all changes
 */
void ACC_SalesTaxPostingDialog::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
}

/**
 * Button add clicked
 */
void ACC_SalesTaxPostingDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mModel->index(row, mModel->fieldIndex("taxauth_id"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("taxcat_id"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("salestaxacct_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    // end NOTE

    tvSalesTaxPosting->setCurrentIndex(mModel->index(row,  mModel->fieldIndex("taxauth_id")));
    tvSalesTaxPosting->scrollTo(tvSalesTaxPosting->currentIndex());
    cbTaxAuthority->setFocus();
}

/**
 * Button delete clicked
 */
void ACC_SalesTaxPostingDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvSalesTaxPosting->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button sales GL account clicked
 */
void ACC_SalesTaxPostingDialog::on_ileSalesTaxAccount_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(
                        index.row(), mModel->fieldIndex("salestaxacct_idx"));
            mModel->setData(idx, obj->getId()
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString());
        }
    }

    dlg->deleteLater();
}

/**
 * Button Help clicked
 */
void ACC_SalesTaxPostingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_SalesTaxPostingDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_SalesTaxPostingDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_SalesTaxPostingDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

