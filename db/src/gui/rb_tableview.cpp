/*****************************************************************
 * $Id: rb_tableview.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Mar 24, 2011 10:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_tableview.h"

#include "db_dialogfactory.h"
#include "rb_mmproxy.h"
#include "rb_toolbuttonbar.h"


/**
 * Constructor
 */
RB_TableView::RB_TableView(QWidget* parent) : QTableView(parent) {
    mToolButtonBar = NULL;
    mModel = NULL;
    mParentModel = NULL;

    mRestartedAtBegin = false;
}

void RB_TableView::setModel(RB_MmProxy* model) {
    QTableView::setModel(model);
    mModel = model;

    if (mModel) {
        if (mToolButtonBar) {
            mToolButtonBar->setEnabled(true);
        }
        mParentModel = mModel->getParentManager();
    } else {
        if (mToolButtonBar) {
            mToolButtonBar->setEnabled(false);
        }
    }
}

void RB_TableView::setToolButtonBar(RB_ToolButtonBar* tbb) {
    if (mToolButtonBar) {
        removeToolButtonBar();
    }

    mToolButtonBar = tbb;

    connect(mToolButtonBar, SIGNAL(addClicked()),
            this, SLOT(slotAdd()));
    connect(mToolButtonBar, SIGNAL(addChildClicked()),
            this, SLOT(slotAddChild()));
    connect(mToolButtonBar, SIGNAL(deleteClicked()),
            this, SLOT(slotDelete()));

    connect(mToolButtonBar, SIGNAL(editClicked()),
            this, SLOT(slotEdit()));
    connect(mToolButtonBar, SIGNAL(copyClicked()),
            this, SLOT(slotCopy()));
    connect(mToolButtonBar, SIGNAL(saveClicked()),
            this, SLOT(slotSave()));
    connect(mToolButtonBar, SIGNAL(revertClicked()),
            this, SLOT(slotRevert()));

    connect(mToolButtonBar, SIGNAL(previousClicked()),
            this, SLOT(slotPrevious()));
    connect(mToolButtonBar, SIGNAL(nextClicked()),
            this, SLOT(slotNext()));
    connect(mToolButtonBar, SIGNAL(findClicked()),
            this, SLOT(slotFind()));
    connect(mToolButtonBar, SIGNAL(filterOnClicked()),
            this, SLOT(slotFilterOn()));
    connect(mToolButtonBar, SIGNAL(filterOffClicked()),
            this, SLOT(slotFilterOff()));

    connect(mToolButtonBar, SIGNAL(demoteClicked()),
            this, SLOT(slotDemote()));
    connect(mToolButtonBar, SIGNAL(promoteClicked()),
            this, SLOT(slotPromote()));

    connect(mToolButtonBar, SIGNAL(upClicked()),
            this, SLOT(slotUp()));
    connect(mToolButtonBar, SIGNAL(downClicked()),
            this, SLOT(slotDown()));
}

void RB_TableView::removeToolButtonBar() {
    disconnect(mToolButtonBar, SIGNAL(addClicked()),
               this, SLOT(slotAdd()));
    disconnect(mToolButtonBar, SIGNAL(addChildClicked()),
               this, SLOT(slotAddChild()));
    disconnect(mToolButtonBar, SIGNAL(deleteClicked()),
               this, SLOT(slotDelete()));

    disconnect(mToolButtonBar, SIGNAL(editClicked()),
               this, SLOT(slotEdit()));
    disconnect(mToolButtonBar, SIGNAL(copyClicked()),
               this, SLOT(slotCopy()));
    disconnect(mToolButtonBar, SIGNAL(saveClicked()),
               this, SLOT(slotSave()));
    disconnect(mToolButtonBar, SIGNAL(revertClicked()),
               this, SLOT(slotRevert()));

    disconnect(mToolButtonBar, SIGNAL(previousClicked()),
               this, SLOT(slotPrevious()));
    disconnect(mToolButtonBar, SIGNAL(nextClicked()),
               this, SLOT(slotNext()));
    disconnect(mToolButtonBar, SIGNAL(findClicked()),
               this, SLOT(slotFind()));
    disconnect(mToolButtonBar, SIGNAL(filterOnClicked()),
               this, SLOT(slotFilterOn()));
    disconnect(mToolButtonBar, SIGNAL(filterOffClicked()),
               this, SLOT(slotFilterOff()));

    disconnect(mToolButtonBar, SIGNAL(demoteClicked()),
               this, SLOT(slotDemote()));
    disconnect(mToolButtonBar, SIGNAL(promoteClicked()),
               this, SLOT(slotPromote()));

    disconnect(mToolButtonBar, SIGNAL(upClicked()),
               this, SLOT(slotUp()));
    disconnect(mToolButtonBar, SIGNAL(downClicked()),
               this, SLOT(slotDown()));

    mToolButtonBar = NULL;
}

void RB_TableView::slotAdd() {
    if(!mModel)  return;
    mModel->undoFilterSort();

    if (mModel->getParentManager()
            && !mModel->getParentManager()->getCurrentIndex().isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog("No parent row selected");
        return;
    }

    // always insert at the beginning
    int row = 0; // mModel->rowCount(); would be at end
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    RB_String objectListName = mModel->getListName();
    RB_String objectName = "";

    if (objectListName.endsWith("List", Qt::CaseInsensitive)) {
        objectName = objectListName.remove("List", Qt::CaseInsensitive);
    } else {
        RB_DEBUG->error("RB_TableView::slotAdd() does not end on List ERROR");
        return;
    }

    RB_ObjectBase* obj = mModel->getObjectFactory()->newSingleObject(objectName);
    int memberCount = obj->memberCount();

    QModelIndex idx;

    for (int col = RB2::HIDDENCOLUMNS - 1; col < memberCount; ++col) {
        idx = mModel->index(row,
                            mModel->fieldIndex(obj->getMember(col)->getName()),
                            QModelIndex());
        mModel->setData(idx, obj->getValue(col), Qt::EditRole);
    }

    delete obj;
    // end NOTE

    setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    scrollTo(currentIndex());
}

void RB_TableView::slotAddChild() {
    DB_DIALOGFACTORY->commandMessage("RB_TableView::slotAddChild() not a treeview ERROR");
    RB_DEBUG->error("RB_TableView::slotAddChild() not a treeview ERROR");
}

void RB_TableView::slotDelete() {
    if (!mModel) return;
    QModelIndex idx = currentIndex();

    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestInformationDialog(tr("No item selected"));
        return;
    }

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) {
        return;
    }

    mModel->removeRows(idx.row(), 1, idx.parent());
}

void RB_TableView::slotEdit() {
    DB_DIALOGFACTORY->requestWarningDialog("Not yet implemented");
}

void RB_TableView::slotCopy() {
    if(!mModel)  return;
    QModelIndex idx = currentIndex();

    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestInformationDialog(tr("No item selected"));
        return;
    }

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Copy item"),
                                          tr("Do you want to copy\n"
                                             "the current item?"));
    if (res != QMessageBox::Yes) {
        return;
    }

    mModel->undoFilterSort();
    mModel->copyCurrentRow();
}

void RB_TableView::slotSave() {
    // Make sure that all lineEdit emits editingFinished()
    setFocus();
    mModel->submitAllAndSelect();
}

void RB_TableView::slotRevert() {
    mModel->revertAll();
}

void RB_TableView::slotPrevious() {
    mModel->getMapper()->toPrevious();
}

void RB_TableView::slotNext() {
    mModel->getMapper()->toNext();
}

void RB_TableView::slotFind() {
    RB_String text = mToolButtonBar->getFindFilterText();
    if (text.isEmpty()) return;
    if (!mModel) return;

    QModelIndex currentIndex = this->currentIndex();
    QModelIndex nextIndex = mModel->index(0, 0, QModelIndex());
    int rowCount = mModel->rowCount();
    int colCount = mModel->columnCount();

    if (currentIndex.isValid()
            && currentIndex.row() <= rowCount - 1
            && currentIndex.column() <= colCount - 1
            && !mRestartedAtBegin) {
        // continue search
        if (currentIndex.column() == colCount - 1
            && currentIndex.row() < rowCount - 1) {
            // before last row on last column
            nextIndex = mModel->index(currentIndex.row() + 1, 0, QModelIndex());
        } else if (currentIndex.column() < colCount - 1) {
            // valid row on before last column
            nextIndex = mModel->index(currentIndex.row(),
                                      currentIndex.column() + 1, QModelIndex());
        }
    }

    // Initialize search
    bool searchInit = false;
    bool found = false;

    for(int row = 0; row < rowCount && !found; row++) {
        for (int col = 0; col < colCount && !found; col++) {
            if (!searchInit) {
                col = nextIndex.column();
                row = nextIndex.row();
                searchInit = true;
            } else {
                nextIndex = mModel->index(row, col, QModelIndex());
            }

            QString str = mModel->data(nextIndex, Qt::DisplayRole).toString();
            QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);

            if (regExp.indexIn(str) >= 0) {
                // found
                currentIndex = nextIndex;
                this->setCurrentIndex(currentIndex);
                col = colCount;
                row = rowCount;
                found = true;
            }
        }
    }

    if (!found && !mRestartedAtBegin) {
        // start from beginning?
        int ret = DB_DIALOGFACTORY->requestYesNoDialog(tr("Find"),
                      tr("Searched to end of document,\n"
                         "continue from the beginning?"));
        if (ret == QMessageBox::Yes) {
            mRestartedAtBegin = true;
            slotFind();
        }
    } else if (!found) {
        DB_DIALOGFACTORY->requestWarningDialog("Finished searching.");
    }

    mRestartedAtBegin = false;
}

/**
 * @brief RB_TableView::slotFilterOn
 * \todo map Integer/Uuid ID field to display text to be able to search
 * or filter these fields
 */
void RB_TableView::slotFilterOn() {
    if (!maybeSave()) {
        return;
    }

    RB_String text = mToolButtonBar->getFindFilterText();

    if (text.isEmpty() || text.contains(" ")) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                        "RB_TableView::slotFilterOn() filter is empty or "
                        "has spaces INFO");
        DB_DIALOGFACTORY->commandMessage("Filter removed because:\nfilter is empty or has spaces");
        slotFilterOff();
        return;
    }

    RB_String tableName = mModel->getListName();
    tableName = tableName.remove("List", Qt::CaseInsensitive);

    QModelIndex idx = this->currentIndex();

    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestInformationDialog("No column selected");
        return;
    }

    int col = idx.column();
    RB_String fieldName = "";
    RB_ObjectBase* obj = mModel->getObject(idx, RB2::ResolveNone);

    for (int i = RB2::HIDDENCOLUMNS; i < obj->memberCount()
         && fieldName.isEmpty(); ++i) {
        RB_String testName = obj->getMember(i)->getName();

        if (mModel->fieldIndex(testName) == col) {
            fieldName = testName;
        }
    }

    RB_String filter = "`" + tableName + "`.`" + fieldName + "` LIKE '";

    if (!text.endsWith("%")) {
        text += "%";
    }

    if (!text.startsWith("%")) {
        text = "%" + text;
    }

    filter += text + "'";
    mModel->setWhere(filter);
    mToolButtonBar->setFilterOffEnabled(true);
}

void RB_TableView::slotFilterOff() {
    mModel->setWhere("");
    mToolButtonBar->setFilterOffEnabled(false);
}

void RB_TableView::slotDemote() {
    DB_DIALOGFACTORY->commandMessage("RB_TableView::slotDemote() not a treeview ERROR");
    RB_DEBUG->error("RB_TableView::slotAddDemote() not a treeview ERROR");
}

void RB_TableView::slotPromote() {
    DB_DIALOGFACTORY->commandMessage("RB_TableView::slotPromote() not a treeview ERROR");
    RB_DEBUG->error("RB_TableView::slotPromote() not a treeview ERROR");
}

/**
 * \todo: to be tested
 * Move item up, only for internal memory models, proxy to be unsorted.
 * Gives unpredictable behaviour with database models
 * @brief RB_TableView::slotUp
 */
void RB_TableView::slotUp() {
    QModelIndex idx = currentIndex();

    if (idx.isValid() && idx.row() > 0) {
        QModelIndex idxTo = mModel->index(idx.row()-1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(idx.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        setCurrentIndex(mModel->index(idx.row() - 1, RB2::HIDDENCOLUMNS, QModelIndex()));
        scrollTo(currentIndex());
    }

}

/**
 * \todo: to be tested
 * Move item down, only for internal memory models, proxy to be unsorted.
 * Gives unpredictable behaviour with database models
 * @brief RB_TableView::slotDown
 */
void RB_TableView::slotDown() {
    QModelIndex idx = currentIndex();

    if (idx.isValid() && idx.row() < mModel->rowCount(QModelIndex()) - 1) {
        QModelIndex idxTo = mModel->index(idx.row()+1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(idx.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        setCurrentIndex(mModel->index(idx.row() + 1, RB2::HIDDENCOLUMNS, QModelIndex()));
        scrollTo(currentIndex());
    }
}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 */
bool RB_TableView::maybeSave() {
    if (mModel->isModelModified()) {
        int ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(
                tr("Data changed ..."),
                tr("The data has been modified.\n"
                   "Do you want to keep your changes?"));
        if (ret == QMessageBox::Yes) {
            slotSave();
            DB_DIALOGFACTORY->statusBarMessage(tr("Data saved"), 2000);
        } else if (ret == QMessageBox::No) {
            slotRevert();
            DB_DIALOGFACTORY->statusBarMessage(tr("Changes reverted"), 2000);
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}
