/*****************************************************************
 * $Id: acc_cogsglpostingdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Apr 05, 2010 11:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_cogsglpostingdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_selectchartmasterdialog.h"
#include "acc_selectstockcategorydialog.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_CogsGlPostingDialog::ACC_CogsGlPostingDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_CogsGlPostingDialog::~ACC_CogsGlPostingDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_CogsGlPostingDialog::~ACC_CogsGlPostingDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_CogsGlPostingDialog::init() {

    setWindowTitle(tr("Edit COGS Postings[*]"));

    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCogsGlPosting);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    mMapper = mModel->getMapper();
    addComboBoxMapping(mModel,"area_id", "ACC_Area", "id",
                       "areacode", cbSalesArea, mMapper);
    mMapper->addMapping(ileStockCategory,
                        mModel->fieldIndex("stkcat_idx"));
    mMapper->addMapping(ileGlAccount,
                        mModel->fieldIndex("glcode_idx"));

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
bool ACC_CogsGlPostingDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
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
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void ACC_CogsGlPostingDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add factor company clicked
 */
void ACC_CogsGlPostingDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("area_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("stkcat_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("glcode_idx"));
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvPostings->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvPostings->scrollTo(tvPostings->currentIndex());
    cbSalesArea->setFocus();
}

/**
 * Button delete factor company clicked
 */
void ACC_CogsGlPostingDialog::on_pbDelete_clicked() {
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
void ACC_CogsGlPostingDialog::on_ileStockCategory_clicked() {
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
 * Button GL account clicked
 */
void ACC_CogsGlPostingDialog::on_ileGlAccount_clicked() {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(
                        index.row(), mModel->fieldIndex("glcode_idx"));
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
void ACC_CogsGlPostingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_CogsGlPostingDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_CogsGlPostingDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_CogsGlPostingDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
