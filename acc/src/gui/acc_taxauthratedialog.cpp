/*****************************************************************
 * $Id: acc_taxauthratedialog.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Feb 22, 2010 12:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxauthratedialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_selectchartmasterdialog.h"
#include "acc_selectstockcategorydialog.h"
#include "acc_qachartmaster.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_TaxAuthRateDialog::ACC_TaxAuthRateDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mMapper = NULL;
    mChildMapper = NULL;
//    mSizeHint = QSize(960, 800);
}

/**
 * Destructor
 */
ACC_TaxAuthRateDialog::~ACC_TaxAuthRateDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mChildMapper; deleted by model
    // delete mMapper;
    delete mChildModel;
    delete mModel;
    RB_DEBUG->print("ACC_TaxAuthRateDialog::~ACC_TaxAuthRateDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_TaxAuthRateDialog::init() {

    setWindowTitle(tr("Edit Tax Authority Rates[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxAuthority);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leTaxAuthDescr, mModel->fieldIndex("description"));

    mMapper->addMapping(leBankName, mModel->fieldIndex("bank"));
    mMapper->addMapping(leBankAccountType, mModel->fieldIndex("bankacctype"));
    mMapper->addMapping(leBankAccount, mModel->fieldIndex("bankacc"));
    mMapper->addMapping(leBankSwift, mModel->fieldIndex("bankswift"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvTaxAuthority, mModel);

    //
    // 1. Set child model, tax percentages
    //
    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxAuthRate);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mChildMapper = mChildModel->getMapper();
    addComboBoxMapping(mChildModel,"dispatchtaxprovince_id", "ACC_TaxProvince", "id",
                       "taxprovincename", cbProvince, mChildMapper);
    addComboBoxMapping(mChildModel,"taxcat_id", "ACC_TaxCategory", "id",
                       "taxcatname", cbTaxCategory, mChildMapper);
    mChildMapper->addMapping(sbTaxRate, mChildModel->fieldIndex("taxrate"));

    //
    // (3. select is done after selecting parent)
    // 4. Connect model to child view
    //
    setFormatTableView(tvTaxAuthorityRates, mChildModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvTaxAuthority->hideColumn(i);
        } else {
            tvTaxAuthority->showColumn(i);

            if (tvTaxAuthority->columnWidth(i) < 5) {
                tvTaxAuthority->setColumnWidth(i, 100);
            }
        }
    }

    colCount = mChildModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvTaxAuthorityRates->hideColumn(i);
        } else {
            tvTaxAuthorityRates->showColumn(i);

            if (tvTaxAuthorityRates->columnWidth(i) < 5) {
                tvTaxAuthorityRates->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_TaxAuthRateDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    bool parentSuccess = false;
    bool childSuccess = false;

    if (withSelect) {
        parentSuccess = mModel->submitAllAndSelect();
        childSuccess = mChildModel->submitAllAndSelect();

    } else {
        parentSuccess = mModel->submitAll();
        childSuccess = mChildModel->submitAll();
    }
    // QApplication::restoreOverrideCursor();

    if (!parentSuccess || !childSuccess) {
        return false;
    }

    ACC_QACHARTMASTER->refresh(); // for example tax percentage is changed
    setWindowModified(false);
    return true;
}

/**
 * Reverse all changes
 */
void ACC_TaxAuthRateDialog::fileRevert() {
    mModel->revert();
    mChildModel->revert();
    setWindowModified(false);
}

/**
 * Button add authority clicked
 */
void ACC_TaxAuthRateDialog::on_pbAddAuthority_clicked() {
    if (!mModel) return;

    if (isWindowModified()) {
        fileSave(true);
    }

    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
//    QModelIndex index = mModel->index(row, mModel->fieldIndex("salestaxglacct_idx"));
//    mModel->setData(index, "0", Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("purchtaxglacct_idx"));
//    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvTaxAuthority->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvTaxAuthority->scrollTo(tvTaxAuthority->currentIndex());
    leTaxAuthDescr->setFocus();
    leTaxAuthDescr->selectAll();
}

/**
 * Button delete authority clicked
 */
void ACC_TaxAuthRateDialog::on_pbDeleteAuthority_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvTaxAuthority->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button add order detail clicked
 */
void ACC_TaxAuthRateDialog::on_pbAddAuthRate_clicked() {
    if (!mChildModel) return;
    mChildModel->undoFilterSort();

    // always insert at the end
    int row = mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mChildModel->index(row, mChildModel->fieldIndex("dispatchtaxprovince_id"));
    mChildModel->setData(index, "0", Qt::EditRole);
    index = mChildModel->index(row, mChildModel->fieldIndex("taxcat_id"));
    mChildModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvTaxAuthorityRates->setCurrentIndex(mChildModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvTaxAuthorityRates->scrollTo(tvTaxAuthorityRates->currentIndex());
    cbProvince->setFocus();
}

/**
 * Button delete authority rate clicked
 */
void ACC_TaxAuthRateDialog::on_pbDeleteAuthRate_clicked() {
    if (!mChildModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvTaxAuthorityRates->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
        mChildModel->submitAllAndSelect();
        // setWindowModified(false) not here because parent could have changed
    }
}

/**
 * Button help clicked
 */
void ACC_TaxAuthRateDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_TaxAuthRateDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_TaxAuthRateDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_TaxAuthRateDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Set selected account
 * @param fieldName field to be set
 * @param obj object with account data
 */
void ACC_TaxAuthRateDialog::setAccount(const QString& fieldName,
                                        RB_MmProxy* model,
                                        RB_ObjectBase* obj) {
    QModelIndex index = model->getProxyIndex();
    QModelIndex idx = model->index(
                index.row(), model->fieldIndex(fieldName));
    model->setData(idx, obj->getId()
                    + obj->getValue("accountcode").toString() + " - "
                    + obj->getValue("accountname").toString());
}
