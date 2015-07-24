/*****************************************************************
 * $Id: acc_accountgroupdialog.cpp 2145 2014-05-18 10:45:32Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_accountgroupdialog.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_cmbdelegate.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_AccountGroupDialog::ACC_AccountGroupDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_AccountGroupDialog::~ACC_AccountGroupDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_AccountGroupDialog::~ACC_AccountGroupDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_AccountGroupDialog::init() {

    setWindowTitle(tr("Edit Account Groups[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAccountGroup);
    mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leAccountGroupName, mModel->fieldIndex("groupname"));
    // NOTE: The next item sequence is also used in ACC_ChartMasterDialog
    // With the setTextList() also the tableView shows correct combobox.
    // Do not use currentIndex although in database but use now currentText
    RB_StringList items;
    items << tr("Balance") << tr("Profit and Loss");
    mModel->setTextList(mModel->fieldIndex("pandl"), items);
    cbAccountGroupType->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbAccountGroupType, mModel->fieldIndex("pandl"),
                            "currentIndex");
    mMapper->addMapping(leSequence, mModel->fieldIndex("sequenceintb"));

    //
    // 3. Select after relations have been set, also for treemodels
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTreeView(trvAccountGroup, mModel);
    trvAccountGroup->setItemDelegateForColumn(mModel->fieldIndex("pandl"),
                                               new RB_CmbDelegate(this, items, mModel));
    readSettings();

    // Hidden columns only for tableviews, now tree view
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        trvAccountGroup->showColumn(i);

        if (trvAccountGroup->columnWidth(i) < 5) {
            trvAccountGroup->setColumnWidth(i, 100);
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() to be done after submitAll()
 */
bool ACC_AccountGroupDialog::fileSave(bool withSelect) {
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
void ACC_AccountGroupDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add clicked
 */
void ACC_AccountGroupDialog::on_pbAddGroup_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvAccountGroup->currentIndex().isValid()) {
        idxParent = trvAccountGroup->currentIndex().parent();
    }
    mModel->insertRows(mModel->rowCount(idxParent), 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    int row = mModel->rowCount(idxParent)-1;
    QModelIndex index = mModel->index(row, mModel->fieldIndex("pandl"), idxParent);
    mModel->setHiddenData(index, "0", Qt::EditRole);
    // end NOTE

    // Root is syssetting
    if (!idxParent.isValid()) {
        index = mModel->index(row, mModel->fieldIndex("parent"), idxParent);
        mModel->setHiddenData(index, "syssetting", Qt::EditRole);
    }

    index = mModel->index(mModel->rowCount(idxParent)-1, 0, idxParent);
    trvAccountGroup->setCurrentIndex(index);
    trvAccountGroup->scrollTo(trvAccountGroup->currentIndex());
    leAccountGroupName->setFocus();
    leAccountGroupName->selectAll();
}

/**
 * Button add sub group clicked
 */
void ACC_AccountGroupDialog::on_pbAddSubGroup_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvAccountGroup->currentIndex().isValid()) {
        idxParent = trvAccountGroup->currentIndex();
    } else {
        on_pbAddGroup_clicked();
        return;
    }
    mModel->insertRows(mModel->rowCount(idxParent), 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mModel->index(mModel->rowCount(idxParent)-1,
                          mModel->fieldIndex("pandl"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    index = mModel->index(mModel->rowCount(idxParent)-1, 0, idxParent);
    trvAccountGroup->setCurrentIndex(index);
    trvAccountGroup->scrollTo(trvAccountGroup->currentIndex());
    leAccountGroupName->setFocus();
    leAccountGroupName->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_AccountGroupDialog::on_pbDeleteGroup_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = trvAccountGroup->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
        fileSave(true);
    }
}

/**
 * Button Help clicked
 */
void ACC_AccountGroupDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_AccountGroupDialog::on_pbOk_clicked() {
    fileSave(false);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_AccountGroupDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_AccountGroupDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
