/*****************************************************************
 * $Id: db_tabledialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jan 31, 2016 8:34:42 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_tablewidget.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
DB_TableWidget::DB_TableWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mCurrentModelIndex = QModelIndex();
    mCurrentObject = NULL;

    mModel = NULL;
}

/**
 * Destructor
 */
DB_TableWidget::~DB_TableWidget() {
    RB_DEBUG->print("DB_TableWidget::~DB_TableWidget() OK");
}

/**
 * File Save, save model changes
 */
bool DB_TableWidget::fileSave(bool withSelect) {
    if (!mModel) return false;
    beforeFileSave();

    mCurrentModelIndex = tableView->currentIndex();

    if (!mCurrentModelIndex.isValid()) {
        mCurrentObject = NULL;
    } else {
        // for selection windows
        setCurrentObject();
    }

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

    RB_DEBUG->print("DB_TableWidget::fileSave() ERROR");
    return false;
}

/**
 * File Revert, reverse modelchanges
 */
void DB_TableWidget::fileRevert() {
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
void DB_TableWidget::setHelpSubject(const RB_String& subject) {
    mHelpSubject = subject;
}

///**
// * Get current selected model index. This is the actual index mapped to the
// * source model.
// * @return selected source model index
// */
//QModelIndex DB_TableWidget::getCurrentModelIndex() const {
//    return mCurrentModelIndex;
//}

/**
 * Get current object based on selected model index.
 * @return object with data from model
 */
RB_ObjectBase* DB_TableWidget::getCurrentObject() {
    return mCurrentObject;
}


void DB_TableWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    tableView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tableView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
}

void DB_TableWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Delete item"), tr("Deletion cannot be undone.\n"
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

void DB_TableWidget::on_pbUp_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QModelIndex idxTo = mModel->index(index.row()-1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tableView->selectRow(index.row() - 1);
    }
}

void DB_TableWidget::on_pbDown_clicked() {
    if (!mModel) return;

    QModelIndex index = tableView->currentIndex();
    if (index.isValid() && index.row() < mModel->rowCount(QModelIndex()) - 1) {
        QModelIndex idxTo = mModel->index(index.row()+1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tableView->selectRow(index.row() + 1);
    }
}

void DB_TableWidget::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTable(searchStr, tableView);
}

void DB_TableWidget::on_pbFilter_clicked() {
    filterTable(tableView);
}

void DB_TableWidget::on_pbFilterClear_clicked() {
    mModel->removeUserFilter();
}

void DB_TableWidget::initEditSort() {
    setFormatTableView(tableView, mModel);
    pbUp->hide();
    pbDown->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TableWidget::initEditUpDown() {
    setFormatTableView(tableView, mModel);
    tableView->setSortingEnabled(false);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TableWidget::initEditOnly() {
    setFormatTableView(tableView, mModel);
    // tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pbAdd->hide();
    pbDelete->hide();
    pbUp->hide();
    pbDown->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TableWidget::initSelectionOnly() {
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
void DB_TableWidget::initTest() {
    setFormatTableView(tableView, mModel);
    leRoot->setReadOnly(true);
}

/**
 * Set current object with data from current selected index
 */
void DB_TableWidget::setCurrentObject() {
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
void DB_TableWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
