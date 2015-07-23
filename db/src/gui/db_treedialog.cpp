/*****************************************************************
 * $Id: db_treedialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Aug 10, 2010 8:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_treedialog.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
DB_TreeDialog::DB_TreeDialog(QWidget *parent)
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
DB_TreeDialog::~DB_TreeDialog() {
    RB_DEBUG->print("DB_TreeDialog::~DB_TreeDialog() OK");
}

/**
 * File Save, save model changes
 */
bool DB_TreeDialog::fileSave(bool withSelect) {
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
    RB_DEBUG->print("DB_TreeDialog::fileSave() ERROR");
    return false;
}

/**
 * File Revert, reverse modelchanges
 */
void DB_TreeDialog::fileRevert() {
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
void DB_TreeDialog::setHelpSubject(const RB_String& subject) {
    mHelpSubject = subject;
}

///**
// * Get current selected model index. This is the actual index mapped to the
// * source model.
// * @return selected source model index
// */
//QModelIndex DB_TreeDialog::getCurrentModelIndex() const {
//    return mCurrentModelIndex;
//}

/**
 * Get current object based on selected model index.
 * @return object
 */
RB_ObjectBase* DB_TreeDialog::getCurrentObject() {
    return mCurrentObject;
}

/**
 * Add sibling item (at same level as selected item)
 */
void DB_TreeDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (treeView->currentIndex().isValid()) {
        idxParent = treeView->currentIndex().parent();
    }

    // always insert at the end
    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    treeView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, idxParent));
    treeView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, idxParent));
}

/**
 * Add child item (at sub level of selected item)
 */
void DB_TreeDialog::on_pbAddChild_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (treeView->currentIndex().isValid()) {
        idxParent = treeView->currentIndex();
    }
    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    treeView->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, idxParent));
    treeView->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, idxParent));
}

void DB_TreeDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = treeView->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);

        if (row > 0) {
            treeView->setCurrentIndex(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            treeView->scrollTo(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

void DB_TreeDialog::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTree(searchStr, treeView);
}

void DB_TreeDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(mHelpSubject);
}

void DB_TreeDialog::on_pbOk_clicked() {
    if (!mModel) return;
    beforeFileSave();

    mCurrentModelIndex = treeView->currentIndex();

    if (!mCurrentModelIndex.isValid()) {
        mCurrentObject = NULL;
    } else {
        setCurrentObject();
    }

    // For selection dialogs only the current object is required
    if (isWindowModified()) {
        mModel->submitAll();
    }

    QDialog::accept();
}

void DB_TreeDialog::on_pbCancel_clicked() {
    mCurrentModelIndex = QModelIndex();
    mCurrentObject = NULL;
    RB_Dialog::reject();
}

void DB_TreeDialog::initEditSort() {
    setFormatTreeView(treeView, mModel);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TreeDialog::initEditUpDown() {
    setFormatTreeView(treeView, mModel);
    treeView->setSortingEnabled(false);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TreeDialog::initEditOnly() {
    setFormatTreeView(treeView, mModel);
    // treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pbAdd->hide();
    pbAddChild->hide();
    pbDelete->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void DB_TreeDialog::initSelectionOnly() {
    setFormatTreeView(treeView, mModel);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pbAdd->hide();
    pbAddChild->hide();
    pbDelete->hide();
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

/**
 * Initialize base dialog for testing only (both sorting and up/down buttons)
 * @param model table view model
 */
void DB_TreeDialog::initTest() {
    setFormatTreeView(treeView, mModel);
    leRoot->setReadOnly(true);
}

/**
 * Set current object with data from current selected index
 * \todo DB_TableDialog uses mModel->getCurrentObject() synchronize both solutions
 */
void DB_TreeDialog::setCurrentObject() {
    if (treeView->currentIndex().isValid()) {
        mCurrentObject = mModel->getObject(mCurrentModelIndex, RB2::ResolveNone);
        // Below does not work because RB_MmProxy does not have internalPointer()
        // mCurrentObject = static_cast<RB_ObjectBase*>(mCurrentModelIndex.internalPointer());
    } else {
        mCurrentObject = NULL;
    }
}

/**
 * Event handler to handle state changes, such as language change
 * @param e event
 */
void DB_TreeDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
