/*****************************************************************
 * $Id: acc_costgroupdialog.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Jun 20, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costgroupdialog.h"

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
ACC_CostGroupDialog::ACC_CostGroupDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_CostGroupDialog::~ACC_CostGroupDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_CostGroupDialog::~ACC_CostGroupDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_CostGroupDialog::init() {

    setWindowTitle(tr("Edit Cost Groups[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCostGroup);
    mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leCostCenterGroupName, mModel->fieldIndex("groupname"));
    // NOTE: The next item sequence is also used in ACC_ChartMasterDialog
    RB_StringList items;
    items << tr("Cost Center") << tr("Profit Center");
    mModel->setTextList(mModel->fieldIndex("costprofit"), items);
    cbGroupType->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbGroupType, mModel->fieldIndex("costprofit"),
                            "currentIndex");
    connect(mModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this, SLOT(slotRowChanged(QModelIndex, QModelIndex)));
    mMapper->addMapping(leSequenceNumber, mModel->fieldIndex("sequenceno"));

    //
    // 3. Select after relations have been set, also for tree models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTreeView(trvCostGroups, mModel);
//    trvCostGroups->setItemDelegateForColumn(mModel->fieldIndex("costprofit"),
//                                            new RB_CmbDelegate(this, items, mModel));
    readSettings();

    // Hidden columns only for tableviews, now tree view
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        trvCostGroups->showColumn(i);

        if (trvCostGroups->columnWidth(i) < 5) {
            trvCostGroups->setColumnWidth(i, 100);
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() to be done after submitAll()
 */
bool ACC_CostGroupDialog::fileSave(bool withSelect) {
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
void ACC_CostGroupDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add clicked
 */
void ACC_CostGroupDialog::on_pbAddGroup_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvCostGroups->currentIndex().isValid()) {
        idxParent = trvCostGroups->currentIndex().parent();
    }
    mModel->insertRows(mModel->rowCount(idxParent), 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
//    QModelIndex index = mModel->index(mModel->rowCount(idxParent)-1,
//                          mModel->fieldIndex("pandl"), idxParent);
//    mModel->setHiddenData(index, "0", Qt::EditRole);
    // end NOTE

    QModelIndex index = mModel->index(mModel->rowCount(idxParent)-1, 0, idxParent);
    trvCostGroups->setCurrentIndex(index);
    trvCostGroups->scrollTo(trvCostGroups->currentIndex());
    leCostCenterGroupName->setFocus();
    leCostCenterGroupName->selectAll();
}

/**
 * Button add sub group clicked
 */
void ACC_CostGroupDialog::on_pbAddSubGroup_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvCostGroups->currentIndex().isValid()) {
        idxParent = trvCostGroups->currentIndex();
    }
    mModel->insertRows(mModel->rowCount(idxParent), 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
//    QModelIndex index = mModel->index(mModel->rowCount(idxParent)-1,
//                          mModel->fieldIndex("pandl"), idxParent);
//    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    QModelIndex index = mModel->index(mModel->rowCount(idxParent)-1, 0, idxParent);
    trvCostGroups->setCurrentIndex(index);
    trvCostGroups->scrollTo(trvCostGroups->currentIndex());
    leCostCenterGroupName->setFocus();
    leCostCenterGroupName->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_CostGroupDialog::on_pbDeleteGroup_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = trvCostGroups->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button Help clicked
 */
void ACC_CostGroupDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_CostGroupDialog::on_pbOk_clicked() {
    fileSave(false);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_CostGroupDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Selected row changed, update combobox
 */
void ACC_CostGroupDialog::slotRowChanged(const QModelIndex& /*current*/,
                                         const QModelIndex& /*previous*/) {
    int currentIndex = mModel->getCurrentValue("costprofit",
                                               RB2::RoleOrigData).toInt();
    cbGroupType->setCurrentIndex(currentIndex);
}

/**
 * Change event, for example translation
 */
void ACC_CostGroupDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
