/*****************************************************************
 * $Id: acc_salesglpostingdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 05, 2010 11:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesglpostingdialog.h"

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
ACC_SalesGlPostingDialog::ACC_SalesGlPostingDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_SalesGlPostingDialog::~ACC_SalesGlPostingDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_SalesGlPostingDialog::~ACC_SalesGlPostingDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_SalesGlPostingDialog::init() {

    setWindowTitle(tr("Edit Sales GL Postings[*]"));

    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesGlPosting);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    mMapper = mModel->getMapper();
    addComboBoxMapping(mModel,"taxauth_id", "ACC_TaxAuthority", "id",
                       "description", cbTaxAuthority, mMapper);
    addComboBoxMapping(mModel,"area_id", "ACC_Area", "id",
                       "areacode", cbSalesArea, mMapper);
    addComboBoxMapping(mModel,"salestype_id", "ACC_SalesType", "id",
                       "typeabbrev", cbSalesType, mMapper);
    mMapper->addMapping(ileStockCategory,
                        mModel->fieldIndex("stkcat_idx"));
    mMapper->addMapping(ileDiscountGlAccount,
                        mModel->fieldIndex("discountglact_idx"));
    mMapper->addMapping(ileSalesGlAccount,
                        mModel->fieldIndex("salesglact_idx"));

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
 * and maybeSave(). Refreshes ACC_QaChartMaster
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_SalesGlPostingDialog::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mModel) {
        if (!mModel->submitAllAndSelect()) {
            QApplication::restoreOverrideCursor();
            return false;
        }
    } else if (mModel) {
        if (!mModel->submitAll()) {
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
void ACC_SalesGlPostingDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add factor company clicked
 */
void ACC_SalesGlPostingDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("taxauth_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("area_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("stkcat_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("salesglact_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("discountglact_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("salestype_id"));
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvPostings->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvPostings->scrollTo(tvPostings->currentIndex());
    cbTaxAuthority->setFocus();
}

/**
 * Button delete factor company clicked
 */
void ACC_SalesGlPostingDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvPostings->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button stock category clicked
 */
void ACC_SalesGlPostingDialog::on_ileStockCategory_clicked() {
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
 * Button discount GL account clicked
 */
void ACC_SalesGlPostingDialog::on_ileDiscountGlAccount_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(
                        index.row(), mModel->fieldIndex("discountglact_idx"));
            mModel->setData(idx, obj->getId()
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString());
        }
    }

    dlg->deleteLater();
}

/**
 * Button sales GL account clicked
 */
void ACC_SalesGlPostingDialog::on_ileSalesGlAccount_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(
                        index.row(), mModel->fieldIndex("salesglact_idx"));
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
void ACC_SalesGlPostingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_SalesGlPostingDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_SalesGlPostingDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_SalesGlPostingDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
