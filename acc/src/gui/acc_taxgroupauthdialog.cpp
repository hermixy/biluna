/*****************************************************************
 * $Id: acc_taxgroupauthdialog.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Feb 24, 2010 12:00:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_taxgroupauthdialog.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_cmbdelegate.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_TaxGroupAuthDialog::ACC_TaxGroupAuthDialog(QWidget *parent)
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
ACC_TaxGroupAuthDialog::~ACC_TaxGroupAuthDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mChildMapper;
    delete mMapper;
    delete mChildModel;
    delete mModel;
    RB_DEBUG->print("ACC_TaxGroupAuthDialog::~ACC_TaxGroupAuthDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_TaxGroupAuthDialog::init() {

    setWindowTitle(tr("Edit Tax Group and Tax Authorities[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxGroup);
    mModel->setRoot(DB_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leTaxGroupName, mModel->fieldIndex("taxgroupdescr"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvTaxGroup, mModel);

    //
    // 1. Set child model
    //
    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxGroupAuth);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mChildMapper = mChildModel->getMapper();
    addComboBoxMapping(mChildModel,"taxauth_id", "ACC_TaxAuthority", "id",
                       "description", cbTaxAuthorityName, mChildMapper);
    mChildMapper->addMapping(leCalculationOrder, mChildModel->fieldIndex("calculationorder"));
    // create and attach fixed comboBox model
    RB_StringList items;
    items << tr("No") << tr("Yes");
    mChildModel->setTextList(mChildModel->fieldIndex("taxontax"), items);
    cbTaxOnPriorTaxes->setModel(new QStringListModel(items, this));
    mChildMapper->addMapping(cbTaxOnPriorTaxes, mChildModel->fieldIndex("taxontax"), "currentIndex");

    //
    // (3. Root will be set by selecting row in parent model)
    // 4. connect to child (table) view
    //
    setFormatTableView(tvTaxAuthority, mChildModel);
    tvTaxAuthority->setItemDelegateForColumn(mChildModel->fieldIndex("taxontax"),
                                               new RB_CmbDelegate(this, items, mChildModel));

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvTaxGroup->hideColumn(i);
        } else {
            tvTaxGroup->showColumn(i);

            if (tvTaxGroup->columnWidth(i) < 5) {
                tvTaxGroup->setColumnWidth(i, 100);
            }
        }
    }

    // TODO: does not work because child model not active.
    colCount = mChildModel->columnCount();
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
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_TaxGroupAuthDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect) {
        mModel->submitAllAndSelect();
        mChildModel->submitAllAndSelect();

    } else {
        mModel->submitAll();
        mChildModel->submitAll();
    }
    setWindowModified(false);
    // QApplication::restoreOverrideCursor();
    return true;
}

/**
 * Reverse all changes
 */
void ACC_TaxGroupAuthDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add taxgroup clicked
 */
void ACC_TaxGroupAuthDialog::on_pbAddTaxGroup_clicked() {
    if (!mModel) return;

    if (isWindowModified()) {
        fileSave(true);
    }

    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
//    QModelIndex index = mModel->index(row, mModel->fieldIndex("salestaxglacct_idx"));
//    mModel->setData(index, "0", Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("purchtaxglaccount"));
//    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvTaxGroup->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvTaxGroup->scrollTo(tvTaxGroup->currentIndex());
    leTaxGroupName->setFocus();
    leTaxGroupName->selectAll();
}

/**
 * Button delete taxgroup clicked
 */
void ACC_TaxGroupAuthDialog::on_pbDeleteTaxGroup_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvTaxGroup->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button add taxauthority clicked
 */
void ACC_TaxGroupAuthDialog::on_pbAddTaxAuthority_clicked() {
    if (!mChildModel) return;
    mChildModel->undoFilterSort();

    // always insert at the end
    int row = mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mChildModel->index(row, mChildModel->fieldIndex("taxauth_id"));
    mChildModel->setData(index, "0", Qt::EditRole);
    index = mChildModel->index(row, mChildModel->fieldIndex("calculationorder"));
    mChildModel->setData(index, 0, Qt::EditRole);
    index = mChildModel->index(row, mChildModel->fieldIndex("taxontax"));
    mChildModel->setData(index, 0, Qt::EditRole);
    // end NOTE

    tvTaxAuthority->setCurrentIndex(mChildModel->index(row, RB2::HIDDENCOLUMNS));
    tvTaxAuthority->scrollTo(tvTaxAuthority->currentIndex());
    cbTaxAuthorityName->setFocus();
}

/**
 * Button delete taxauthority clicked
 */
void ACC_TaxGroupAuthDialog::on_pbDeleteTaxAuthority_clicked() {
    if (!mChildModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvTaxAuthority->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
        mChildModel->submitAllAndSelect();
        // setWindowModified(false); not here because parent could have changed
    }
}

/**
 * Button help clicked
 */
void ACC_TaxGroupAuthDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_TaxGroupAuthDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void ACC_TaxGroupAuthDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_TaxGroupAuthDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
