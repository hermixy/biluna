/*****************************************************************
 * $Id: acc_chartmasterdialog.cpp 2145 2014-05-18 10:45:32Z rutger $
 * Created: Feb 25, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_chartmasterdialog.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_qachartmaster.h"
#include "acc_sqlcommonfunctions.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_ChartMasterDialog::ACC_ChartMasterDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_ChartMasterDialog::~ACC_ChartMasterDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mMapper; mappers are deleted when model is deleted
    delete mModel; // the unique model is used, therefore delete here
    RB_DEBUG->print("ACC_ChartMasterDialog::~ACC_ChartMasterDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_ChartMasterDialog::init() {

    setWindowTitle(tr("Edit Chart of Accounts[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    // Shared is false because ChartMaster is unfiltered compared to other
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelChartMaster, false);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    //
    // Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leAccountCode, mModel->fieldIndex("accountcode"));
    mMapper->addMapping(leAccountName, mModel->fieldIndex("accountname"));
    addComboBoxMapping(mModel, "accountgroup_id", "ACC_AccountGroup", "id",
                       "groupname", cbAccountGroup, mMapper);
    // Above line already created the relational model, next connection line
    // uses the relational model to retrieve other values
    connect(cbAccountGroup, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotAccountGroupChanged(int)));
    addComboBoxMapping(mModel, "accountcontrol", "ACC_AccountControl", "controlnumber",
                       "description", cbAccountControl, mMapper);

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvChart, mModel);

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS || i == mModel->fieldIndex("accountrequired")) {
            tvChart->hideColumn(i);
        } else {
            tvChart->showColumn(i);

            if (tvChart->columnWidth(i) < 5) {
                tvChart->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave(). Refreshes ACC_QaChartMaster.
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_ChartMasterDialog::fileSave(bool withSelect) {
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
void ACC_ChartMasterDialog::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
}

/**
 * Button add clicked
 */
void ACC_ChartMasterDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mModel->index(row, mModel->fieldIndex("accountgroup_id"), QModelIndex());
    mModel->setData(idx, "0", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("accountrequired"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("accountcontrol"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tvChart->setCurrentIndex(mModel->index(row,  mModel->fieldIndex("accountcode")));
    tvChart->scrollTo(tvChart->currentIndex());
    leAccountCode->setFocus();
    leAccountCode->selectAll();
}

/**
 * Button delete clicked
 */
void ACC_ChartMasterDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    QModelIndex index = tvChart->currentIndex();
    if (!index.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog("No account selected.");
        return;
    }

    // Check whether bookings have been done this current year to this account
    RB_String accountId = mModel->getCurrentId();
    ACC_SqlCommonFunctions oper;

    if (oper.isAccountUsed(accountId)) {
        ACC_DIALOGFACTORY->requestWarningDialog("Cannot delete used account.");
        return;
    }


    // Check whether this is a required account
    QModelIndex idx = mModel->index(index.row(), mModel->fieldIndex("accountrequired"));
    int required = mModel->data(idx, Qt::EditRole).toInt();
    if (required) {
        ACC_DIALOGFACTORY->requestWarningDialog("Cannot delete required account.");
        return;
    }

    // Last warning
    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    mModel->removeRows(index.row(), 1, QModelIndex());
    mModel->submitAllAndSelect();
    setWindowModified(true);
}

/**
 * Button Help clicked
 */
void ACC_ChartMasterDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_ChartMasterDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_ChartMasterDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Slot for accountgroup changed
 */
void ACC_ChartMasterDialog::slotAccountGroupChanged(int row) {
    // Relational model was already created in init()
    QSqlTableModel* relModel = mModel->relationModel(mModel->fieldIndex("accountgroup_id"));
    QModelIndex idx = relModel->index(row, relModel->fieldIndex("pandl"));
    int number = relModel->data(idx).toInt();

    // NOTE: The next item sequence is set in ACC_AccountGroupDialog
    switch (number) {
    case 0:
        leGroupType->setText(tr("Balance"));
        break;
    case 1:
        leGroupType->setText(tr("Profit and Loss"));
        break;
    default:
        leGroupType->setText(tr("Undefined"));
        break;
    }
}

/**
 * Change event, for example translation
 */
void ACC_ChartMasterDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
