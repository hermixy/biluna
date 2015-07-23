/*****************************************************************
 * $Id: rb_dialogbase.cpp 2150 2014-06-16 20:05:21Z rutger $
 * Created: Oct 26, 2009 8:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_dialogbase.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
RB_DialogBase::RB_DialogBase(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mCurrentModelIndex = QModelIndex();

    mModel = NULL;
    mMainWindow = dynamic_cast<RB_MainWindow*>(parent);
}

/**
 * Destructor
 */
RB_DialogBase::~RB_DialogBase() {
//    delete mModel; if required by implementation.
    RB_DEBUG->print("RB_DialogBase::~RB_DialogBase() OK");
}

/**
 * File Save, save model changes
 */
bool RB_DialogBase::fileSave(bool /*withSelect*/) {
    if (mModel && mModel->submitAll()) {
        setWindowModified(false);
        return true;
    }
    return false;
}

/**
 * File Revert, reverse modelchanges
 */
void RB_DialogBase::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
}

/**
 * Set help subject for 'Help' button. For a dialog this is usually
 * the dialog class name
 * @param subject help subject
 */
void RB_DialogBase::setHelpSubject(const RB_String& subject) {
    mHelpSubject = subject;
}

/**
 * Get current selected model index. This is the actual index mapped to the
 * source model.
 * @return selected source model index
 */
QModelIndex RB_DialogBase::getCurrentModelIndex() const {
    return mCurrentModelIndex;
}

RB_MmProxy* RB_DialogBase::getModel() {
    return mModel;
}

void RB_DialogBase::on_pbAdd_clicked() {
    if (!mModel) return;

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());
    tableView->setCurrentIndex(mModel->index(row, 0, QModelIndex()));
    tableView->scrollTo(tableView->currentIndex());
}

void RB_DialogBase::on_pbDelete_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

void RB_DialogBase::on_pbUp_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QModelIndex idxTo = mModel->index(index.row()-1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tableView->selectRow(index.row() - 1);
    }
}

void RB_DialogBase::on_pbDown_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid() && index.row() < mModel->rowCount(QModelIndex()) - 1) {
        QModelIndex idxTo = mModel->index(index.row()+1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tableView->selectRow(index.row() + 1);
    }
}

void RB_DialogBase::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTable(searchStr, tableView);
}

void RB_DialogBase::on_pbFilter_clicked() {
    filterTable(tableView);
}

void RB_DialogBase::on_pbFilterClear_clicked() {
    mModel->removeUserFilter();
}

void RB_DialogBase::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(mHelpSubject);
}

void RB_DialogBase::on_pbOk_clicked() {
    if (!mModel) return;
    beforeFileSave();
    fileSave();

    mCurrentModelIndex = tableView->currentIndex();
    if (mCurrentModelIndex.isValid()) {
        QDialog::accept();
    } else {
        QDialog::reject();
    }
}

/**
 * Event handler to handle state changes, such as language change
 * @param e event
 */
void RB_DialogBase::changeEvent(QEvent* e) {
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
 * Initialize the dialog widgets,
 * hides combobox cbSourceModelFilter but connects to slotSourceFilterChange()
 */
void RB_DialogBase::initDialog() {
    tableView->setAlternatingRowColors(true);
    tableView->setEditTriggers(QAbstractItemView::EditKeyPressed |
                                QAbstractItemView::DoubleClicked);
    tableView->verticalHeader()->setDefaultSectionSize(20); // row height
    leRoot->setReadOnly(true);

    cbSourceModelFilter->hide(); // default
}

/**
 * Initialize edit dialog and enables sorting,
 * hides pushbuttons up and down
 */
void RB_DialogBase::initEditSort() {
    tableView->setModel(mModel);
    tableView->setSortingEnabled(true);
    pbUp->hide();
    pbDown->hide();
    initDialog();
}

/**
 * Initialize edit dialog and disable sorting, allow up and down of items
 */
void RB_DialogBase::initEditUpDown() {
    tableView->setModel(mModel);
    tableView->setSortingEnabled(false);
    initDialog();
}

/**
 * Initialize selection dialog and enables sorting, disable editing
 * hides pushbuttons add, delete, up and down
 */
void RB_DialogBase::initSelectionOnly() {
    tableView->setModel(mModel);
    tableView->setSortingEnabled(true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pbAdd->hide();
    pbDelete->hide();
    pbUp->hide();
    pbDown->hide();
    initDialog();
}

/**
 * Initialize base dialog for testing only (both sorting and up/down buttons)
 * @param model table view model
 */
void RB_DialogBase::initTest() {
    tableView->setModel(mModel);
    tableView->setSortingEnabled(true);
    initDialog();
}

