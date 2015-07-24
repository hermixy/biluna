/*****************************************************************
 * $Id: acc_costcenterbudgetdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 22, 2010 12:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costcenterbudgetdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_selectcostgroupdialog.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_CostCenterBudgetDialog::ACC_CostCenterBudgetDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mMapper = NULL;
    mChildMapper = NULL;
}

/**
 * Destructor
 */
ACC_CostCenterBudgetDialog::~ACC_CostCenterBudgetDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mChildMapper; deleted by model
    // delete mMapper;
    delete mChildModel;
    delete mModel;
    RB_DEBUG->print("ACC_CostCenterBudgetDialog::~ACC_CostCenterBudgetDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_CostCenterBudgetDialog::init() {

    setWindowTitle(tr("Edit Cost Center and Budgets[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCostCenter);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leCode, mModel->fieldIndex("centercode"));
    mMapper->addMapping(leName, mModel->fieldIndex("centername"));
    mMapper->addMapping(ileGroup, mModel->fieldIndex("centergroup_idx"));
    mMapper->addMapping(teDescription, mModel->fieldIndex("description"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    // mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvCostCenter, mModel);

    //
    // 1. Set child model, tax percentages
    //
    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCostBudget);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mChildMapper = mChildModel->getMapper();
    mChildMapper->addMapping(lePeriod, mChildModel->fieldIndex("period"));
    mChildMapper->addMapping(leNote, mChildModel->fieldIndex("note"));
    mChildMapper->addMapping(leBudget, mChildModel->fieldIndex("budget"));
    mChildMapper->addMapping(leForecast, mChildModel->fieldIndex("forecast"));

    //
    // (3. select is done after selecting parent)
    // 4. Connect model to child view
    //
    setFormatTableView(tvBudget, mChildModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvCostCenter->hideColumn(i);
        } else {
            tvCostCenter->showColumn(i);

            if (tvCostCenter->columnWidth(i) < 5) {
                tvCostCenter->setColumnWidth(i, 100);
            }
        }
    }

    colCount = mChildModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvBudget->hideColumn(i);
        } else {
            tvBudget->showColumn(i);

            if (tvBudget->columnWidth(i) < 5) {
                tvBudget->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_CostCenterBudgetDialog::fileSave(bool withSelect) {
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

    setWindowModified(false);
    return true;
}

/**
 * Reverse all changes
 */
void ACC_CostCenterBudgetDialog::fileRevert() {
    mModel->revert();
    mChildModel->revert();
    setWindowModified(false);
}

/**
 * Button add cost center clicked
 */
void ACC_CostCenterBudgetDialog::on_pbAddCenter_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    if (isWindowModified()) {
        fileSave(true);
    }

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

    tvCostCenter->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvCostCenter->scrollTo(tvCostCenter->currentIndex());
    leCode->setFocus();
    leCode->selectAll();
}

/**
 * Button delete cost center clicked
 */
void ACC_CostCenterBudgetDialog::on_pbDeleteCenter_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvCostCenter->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button select cost group clicked
 */
void ACC_CostCenterBudgetDialog::on_ileGroup_clicked() {
    if (!tvCostCenter->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center selected.\n"
                                                   "Please select a cost center first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCostGroup);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvCostCenter->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("centergroup_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("groupname").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No group selected.\n"
                                                   "Cost center group is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button add budget clicked
 */
void ACC_CostCenterBudgetDialog::on_pbAddBudget_clicked() {
    if (!mChildModel) return;
    mChildModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
//    QModelIndex index = mChildModel->index(row, mChildModel->fieldIndex("dispatchtaxprovince_id"));
//    mChildModel->setData(index, "0", Qt::EditRole);
//    index = mChildModel->index(row, mChildModel->fieldIndex("taxcat_id"));
//    mChildModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvBudget->setCurrentIndex(mChildModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvBudget->scrollTo(tvBudget->currentIndex());
    lePeriod->setFocus();
    lePeriod->selectAll();
}

/**
 * Button delete budget clicked
 */
void ACC_CostCenterBudgetDialog::on_pbDeleteBudget_clicked() {
    if (!mChildModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvBudget->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
        mChildModel->submitAllAndSelect();
        // setWindowModified(true); not here because parent could have changes
    }
}

/**
 * Button calculate actual clicked. Calculates and shows
 * the actual value based on data in the database
 */
void ACC_CostCenterBudgetDialog::on_pbCalculateActual_clicked() {
    ACC_DIALOGFACTORY->requestInformationDialog("To be implemented");

    // check if more periods in this year or only 201201
    // select from acc_gltrans with period limit
    // or like acc_glsum with acc_costsum fixed per month
}

/**
 * Button help clicked
 */
void ACC_CostCenterBudgetDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_CostCenterBudgetDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_CostCenterBudgetDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_CostCenterBudgetDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

