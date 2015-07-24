/*****************************************************************
 * $Id: acc_accountcontroldialog.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Nov 11, 2010 4:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_accountcontroldialog.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_qasysacctcontrol.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_AccountControlDialog::ACC_AccountControlDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
    mSysModel = NULL;
}

/**
 * Destructor
 */
ACC_AccountControlDialog::~ACC_AccountControlDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mMapper; mappers are deleted when model is deleted
    delete mModel; // the unique model is used, therefore delete here
    RB_DEBUG->print("ACC_AccountControlDialog::~ACC_AccountControlDialog() OK");
}

/**
 * Initialize widget and models
 */
void ACC_AccountControlDialog::init() {

    setWindowTitle(tr("Edit Account Control[*]"));

    //
    // 1. Set model with ID (sets root object) and/or query
    // Shared is false because ChartMaster is unfiltered compared to other
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAccountControl, false);
    mModel->setRoot("syssetting" /*ACC_MODELFACTORY->getRootId()*/);

    //
    // Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();

    mSysModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSystemAccountControl);
    cbSysAcctControl->setModel(mSysModel);
    cbSysAcctControl->setModelColumn(mSysModel->fieldIndex("description"));

    // Does not work here because currentIndex is not the same as the enumerator
//    RB_StringList items;
//    ACC_QASYSACCTCONTROL->setAccountControlList(items);
//    mModel->setTextList(mModel->fieldIndex("controlnumber"), items);
//    cbSysAcctControl->setModel(new QStringListModel(items, this));
//    mMapper->addMapping(cbSysAcctControl, mModel->fieldIndex("controlnumber"),
//                            "currentIndex");

    mMapper->addMapping(this->leAcctControlName, mModel->fieldIndex("description"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvAccountControl, mModel);

    connect(tvAccountControl->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotSelectionChanged(QModelIndex, QModelIndex)));
    connect(cbSysAcctControl, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotIndexChanged(int)));

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvAccountControl->hideColumn(i);
        } else {
            tvAccountControl->showColumn(i);

            if (tvAccountControl->columnWidth(i) < 5) {
                tvAccountControl->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool ACC_AccountControlDialog::fileSave(bool withSelect) {
//    QApplication::setOverrideCursor(Qt::WaitCursor);
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
//    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void ACC_AccountControlDialog::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
}

/**
 * Button add clicked
 */
void ACC_AccountControlDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx = mModel->index(row, mModel->fieldIndex("controlnumber"), QModelIndex());
    mModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tvAccountControl->setCurrentIndex(mModel->index(row,  mModel->fieldIndex("controlnumber")));
    tvAccountControl->scrollTo(tvAccountControl->currentIndex());
    cbSysAcctControl->setFocus();
}

/**
 * Button delete clicked
 */
void ACC_AccountControlDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvAccountControl->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button Help clicked
 */
void ACC_AccountControlDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_AccountControlDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_AccountControlDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void ACC_AccountControlDialog::changeEvent(QEvent *e) {
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
 * Table view clicked, update combobox system control
 * @param current selected index
 * @param previous selected index, not used
 */
void ACC_AccountControlDialog::slotSelectionChanged(const QModelIndex& current,
                                                    const QModelIndex& /*previous*/) {
    QModelIndex idx = mModel->index(current.row(), mModel->fieldIndex("controlnumber"));
    int control = idx.data().toInt();

    bool found = false;
    int column = mSysModel->fieldIndex("controlnumber");
    int rowCount = mSysModel->rowCount();

    for (int i = 0; i < rowCount && !found; ++i) {
        idx = mSysModel->index(i, column);
        if (idx.data().toInt() == control) {
            cbSysAcctControl->setCurrentIndex(i);
            found = true;
        }
    }
}

/**
 * Combobox system account control index changed, update model value
 * @param current selected index
 */
void ACC_AccountControlDialog::slotIndexChanged(int index) {
    if (!tvAccountControl->currentIndex().isValid()) {
        return;
    }

    QModelIndex idx = mSysModel->index(index, mSysModel->fieldIndex("controlnumber"));
    mModel->setCurrentValue("controlnumber", idx.data().toInt(), Qt::EditRole);
}
