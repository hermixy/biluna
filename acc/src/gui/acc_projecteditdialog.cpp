/*****************************************************************
 * $Id: acc_projecteditdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_projecteditdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_selectchartmasterdialog.h"
#include "acc_selecttaxprovincedialog.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_sqlrelationaldelegate.h"


/**
 * Constructor
 */
ACC_ProjectEditDialog::ACC_ProjectEditDialog(QWidget *parent)
                        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_ProjectEditDialog::~ACC_ProjectEditDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_ProjectEditDialog::~ACC_ProjectEditDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_ProjectEditDialog::init() {

    setWindowTitle(tr("Edit Companies[*]"));

    //
    // 1. Set model with ID
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelProject, false);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leCompanyName, mModel->fieldIndex("company"));
    mMapper->addMapping(leTaxAuthRef, mModel->fieldIndex("gstno"));
    mMapper->addMapping(leOfficalCoyNo, mModel->fieldIndex("companynumber"));
    mMapper->addMapping(leAddressLine1, mModel->fieldIndex("regoffice1"));
    mMapper->addMapping(leAddressLine2, mModel->fieldIndex("regoffice2"));
    mMapper->addMapping(leAddressLine3, mModel->fieldIndex("regoffice3"));
    mMapper->addMapping(leAddressLine4, mModel->fieldIndex("regoffice4"));
    mMapper->addMapping(leAddressLine5, mModel->fieldIndex("regoffice5"));
    mMapper->addMapping(leAddressLine6, mModel->fieldIndex("regoffice6"));
    mMapper->addMapping(leTelephone, mModel->fieldIndex("telephone"));
    mMapper->addMapping(leFacsimile, mModel->fieldIndex("fax"));
    mMapper->addMapping(leEmail, mModel->fieldIndex("email"));
    mMapper->addMapping(leLogo, mModel->fieldIndex("logo"));
    addComboBoxMapping(mModel,"currencydefault_id", "ACC_Currency", "id",
                       "currcode", cbHomeCurrency, mMapper);
    // Accounting
    mMapper->addMapping(ilePayroll, mModel->fieldIndex("payrollact_idx"));
    mMapper->addMapping(ileGoodsReceived, mModel->fieldIndex("grnact_idx"));
    mMapper->addMapping(ileRetainedEarning, mModel->fieldIndex("retainedearnings_idx"));
    mMapper->addMapping(ileFreight, mModel->fieldIndex("freightact_idx"));
    mMapper->addMapping(ileSalesExchVar, mModel->fieldIndex("exchangediffact_idx"));
    mMapper->addMapping(ilePurchExchVar, mModel->fieldIndex("purchasesexchangediffact_idx"));
    mMapper->addMapping(ilePaymentDiscount, mModel->fieldIndex("pytdiscountact_idx"));
    addComboBoxMapping(mModel,"taxprovince_id", "ACC_TaxProvince", "id",
                       "taxprovincename", cbTaxProvince, mMapper);
    addComboBoxMapping(mModel,"taxgroup_id", "ACC_TaxGroup", "id",
                       "taxgroupdescr", cbTaxGroup, mMapper);
    mMapper->addMapping(leBankName, mModel->fieldIndex("bankname"));
    mMapper->addMapping(leBankIban, mModel->fieldIndex("bankiban"));
    mMapper->addMapping(leBankBicSwift, mModel->fieldIndex("bankbicswift"));
    mMapper->addMapping(sbMonth, mModel->fieldIndex("fiscalmonthstart"));
    mMapper->addMapping(sbDay, mModel->fieldIndex("fiscaldaystart"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));


    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvCompany, mModel);
    // tvCompany->setItemDelegate(new DB_TeBlockDelegate(this));
    readSettings();

    int colCount = mModel->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i != mModel->fieldIndex("company") && i != mModel->fieldIndex("regoffice1")
                && i != mModel->fieldIndex("regoffice2") && i != mModel->fieldIndex("regoffice3")
                && i != mModel->fieldIndex("regoffice4") && i != mModel->fieldIndex("regoffice5")
                && i != mModel->fieldIndex("regoffice6")) {
            tvCompany->hideColumn(i);
        } else {
            tvCompany->showColumn(i);

            if (tvCompany->columnWidth(i) < 5) {
                tvCompany->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_ProjectEditDialog::fileSave(bool withSelect) {
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {
            setWindowModified(false);
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    //QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void ACC_ProjectEditDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

void ACC_ProjectEditDialog::on_pbAdd_clicked() {
    if(!mModel)  return;
    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mModel->index(row, mModel->fieldIndex("currencydefault_id"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("payrollact_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("grnact_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("retainedearnings_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("freightact_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("exchangediffact_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("purchasesexchangediffact_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("pytdiscountact_idx"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("gllink_debtors"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("gllink_creditors"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("gllink_stock"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("taxprovince_id"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("taxgroup_id"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("fiscalmonthstart"), QModelIndex());
    mModel->setData(idx, "1", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("fiscaldaystart"), QModelIndex());
    mModel->setData(idx, "1", Qt::EditRole);
    // end NOTE

    tvCompany->setCurrentIndex(mModel->index(row, mModel->fieldIndex("company"), QModelIndex()));
    tvCompany->scrollTo(tvCompany->currentIndex());
    tabWidget->setCurrentIndex(0);
    leCompanyName->setFocus();
    leCompanyName->selectAll();
}

/**
 * Button delete company clicked
 */
void ACC_ProjectEditDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvCompany->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button help clicked
 */
void ACC_ProjectEditDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_ProjectEditDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_ProjectEditDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Button payroll control clicked
 */
void ACC_ProjectEditDialog::on_ilePayroll_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("payrollact_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Button goods received control clicked
 */
void ACC_ProjectEditDialog::on_ileGoodsReceived_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("grnact_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Button retained earnings control clicked
 */
void ACC_ProjectEditDialog::on_ileRetainedEarning_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("retainedearnings_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Button freight control clicked
 */
void ACC_ProjectEditDialog::on_ileFreight_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("freightact_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Button sales exchange variance control clicked
 */
void ACC_ProjectEditDialog::on_ileSalesExchVar_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("exchangediffact_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Button purchase exchange variance control clicked
 */
void ACC_ProjectEditDialog::on_ilePurchExchVar_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("purchasesexchangediffact_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Button payment discount control clicked
 */
void ACC_ProjectEditDialog::on_ilePaymentDiscount_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            setAccount("pytdiscountact_idx", obj);
        }
    }

    dlg->deleteLater();
}

/**
 * Change event, for example translation
 */
void ACC_ProjectEditDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
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
void ACC_ProjectEditDialog::setAccount(const QString& fieldName,
                                       RB_ObjectBase* obj) {
    QModelIndex index = mModel->getProxyIndex();
    QModelIndex idx = mModel->index(
                index.row(), mModel->fieldIndex(fieldName));
    mModel->setData(idx, obj->getId()
                    + obj->getValue("accountcode").toString() + " - "
                    + obj->getValue("accountname").toString());
}
