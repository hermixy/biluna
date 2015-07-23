/*****************************************************************
 * $Id: db_tabledialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Aug 8, 2010 8:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_tabledialog.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
DB_TableDialog::DB_TableDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mCurrentModelIndex = QModelIndex();
    mCurrentObject = NULL;

    mModel = NULL;
    readSettings();
}

/**
 * Destructor
 */
DB_TableDialog::~DB_TableDialog() {
    RB_DEBUG->print("DB_TableDialog::~DB_TableDialog() OK");
}

/**
 * File Save, save model changes
 */
bool DB_TableDialog::fileSave(bool withSelect) {
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
    RB_DEBUG->print("DB_TableDialog::fileSave() ERROR");
    return false;
}

/**
 * File Revert, reverse modelchanges
 */
void DB_TableDialog::fileRevert() {
    if (mModel) {
        mModel->revertAll();
        setWindowModified(false);
    }
}

/**
 * Set help subject for 'Help' button. For a dialog this is usually
 * the dialog class name
 * @param subject help subject
 */
void DB_TableDialog::setHelpSubject(const RB_String& subject) {
    mHelpSubject = subject;
}

///**
// * Get current selected model index. This is the actual index mapped to the
// * source model.
// * @return selected source model index
// */
//QModelIndex DB_TableDialog::getCurrentModelIndex() const {
//    return mCurrentModelIndex;
//}

/**
 * Get current object based on selected model index.
 * @return object with data from model
 */
RB_ObjectBase* DB_TableDialog::getCurrentObject() {
    return mCurrentObject;
}


void DB_TableDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
}

void DB_TableDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tableView->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);

        if (row > 0) {
            tableView->setCurrentIndex(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tableView->scrollTo(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

void DB_TableDialog::on_pbUp_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QModelIndex idxTo = mModel->index(index.row()-1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tableView->selectRow(index.row() - 1);
    }
}

void DB_TableDialog::on_pbDown_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid() && index.row() < mModel->rowCount(QModelIndex()) - 1) {
        QModelIndex idxTo = mModel->index(index.row()+1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tableView->selectRow(index.row() + 1);
    }
}

void DB_TableDialog::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTable(searchStr, tableView);
}

void DB_TableDialog::on_pbFilter_clicked() {
    filterTable(tableView);
}

void DB_TableDialog::on_pbFilterClear_clicked() {
    mModel->removeUserFilter();
}

void DB_TableDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(mHelpSubject);
}

void DB_TableDialog::on_pbOk_clicked() {
    if (!mModel) return;
    beforeFileSave();

    mCurrentModelIndex = tableView->currentIndex();

    if (!mCurrentModelIndex.isValid()) {
        mCurrentObject = NULL;
    } else {
        // for selection windows
        setCurrentObject();
    }

    if (isWindowModified()) {
        mModel->submitAll();
    }
    QDialog::accept();
}

void DB_TableDialog::on_pbCancel_clicked() {
    mCurrentModelIndex = QModelIndex();
    mCurrentObject = NULL;
    RB_Dialog::reject();
}

void DB_TableDialog::initEditSort() {
    setFormatTableView(tableView, mModel);
    pbUp->hide();
    pbDown->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TableDialog::initEditUpDown() {
    setFormatTableView(tableView, mModel);
    tableView->setSortingEnabled(false);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TableDialog::initEditOnly() {
    setFormatTableView(tableView, mModel);
    // tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pbAdd->hide();
    pbDelete->hide();
    pbUp->hide();
    pbDown->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TableDialog::initSelectionOnly() {
    setFormatTableView(tableView, mModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pbAdd->hide();
    pbDelete->hide();
    pbUp->hide();
    pbDown->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

/**
 * Initialize base dialog for testing only (both sorting and up/down buttons)
 * @param model table view model
 */
void DB_TableDialog::initTest() {
    setFormatTableView(tableView, mModel);
    leRoot->setReadOnly(true);
}

/**
 * Set current object with data from current selected index
 */
void DB_TableDialog::setCurrentObject() {
    if (tableView->currentIndex().isValid()) {
        mCurrentObject = mModel->getCurrentObject();
    } else {
        mCurrentObject = NULL;
    }
}

/**
 * Event handler to handle state changes, such as language change
 * @param e event
 */
void DB_TableDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
