/*****************************************************************
 * $Id: rb_treeview.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Mar 24, 2011 10:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_treeview.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_mmproxy.h"
#include "rb_toolbuttonbar.h"


/**
 * Constructor
 */
RB_TreeView::RB_TreeView(QWidget* parent) : QTreeView(parent) {
    mToolButtonBar = NULL;
    mModel = NULL;
    mParentModel = NULL;

    mRestartedAtBegin = false;
}

void RB_TreeView::setModel(RB_MmProxy* model) {
    QTreeView::setModel(model);
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

void RB_TreeView::setToolButtonBar(RB_ToolButtonBar* tbb) {
    if (mToolButtonBar) {
        removeToolButtonBar();
    }

    mToolButtonBar = tbb;

    connect(mToolButtonBar, SIGNAL(addClicked()),
            this, SLOT(slotAdd()));
    connect(mToolButtonBar, SIGNAL(addChildClicked()),
            this, SLOT(slotAdd()));
    connect(mToolButtonBar, SIGNAL(deleteClicked()),
            this, SLOT(slotDelete()));

    connect(mToolButtonBar, SIGNAL(editClicked()),
            this, SLOT(slotEdit()));
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

void RB_TreeView::removeToolButtonBar() {
    disconnect(mToolButtonBar, SIGNAL(addClicked()),
               this, SLOT(slotAdd()));
    disconnect(mToolButtonBar, SIGNAL(addChildClicked()),
               this, SLOT(slotAdd()));
    disconnect(mToolButtonBar, SIGNAL(deleteClicked()),
               this, SLOT(slotDelete()));

    disconnect(mToolButtonBar, SIGNAL(editClicked()),
               this, SLOT(slotEdit()));
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

void RB_TreeView::slotAdd() {
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
        RB_DEBUG->error("RB_TreeView::slotAdd() does not end on List ERROR");
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

void RB_TreeView::slotAddChild() {
    if (!mModel) return;
    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (currentIndex().isValid()) {
        idxParent = currentIndex().parent();
    } else {
        slotAdd();
        return;
    }
    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    RB_String objectListName = mModel->getListName();
    RB_String objectName = "";

    if (objectListName.endsWith("List", Qt::CaseInsensitive)) {
        objectName = objectListName.remove("List", Qt::CaseInsensitive);
    } else {
        RB_DEBUG->error("RB_TreeView::slotAddChild() does not end on List ERROR");
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

    // Root is current DB_Project.id because it is a system setting
    if (!idxParent.isValid()) {
        idx = mModel->index(row, mModel->fieldIndex("parent"), idxParent);
        mModel->setHiddenData(idx, DB_MODELFACTORY->getRootId(), Qt::EditRole);
    }

    idx = mModel->index(mModel->rowCount(idxParent)-1, RB2::HIDDENCOLUMNS, idxParent);
    setCurrentIndex(idx);
    scrollTo(currentIndex());
}

void RB_TreeView::slotDelete() {
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

void RB_TreeView::slotEdit() {
    DB_DIALOGFACTORY->requestWarningDialog("Not yet implemented");
}

void RB_TreeView::slotSave() {
    // Make sure that all lineEdit emits editingFinished()
    setFocus();
    mModel->submitAllAndSelect();
}

void RB_TreeView::slotRevert() {
    mModel->revertAll();
}

void RB_TreeView::slotPrevious() {
    mModel->getMapper()->toPrevious();
}

void RB_TreeView::slotNext() {
    mModel->getMapper()->toNext();
}

void RB_TreeView::slotFind() {
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
 * @brief RB_TreeView::slotFilterOn
 * \todo map Integer/Uuid ID field to display text to be able to search
 * or filter these fields
 */
void RB_TreeView::slotFilterOn() {
    if (!maybeSave()) {
        return;
    }

    RB_String text = mToolButtonBar->getFindFilterText();

    if (text.isEmpty() || text.contains(" ")) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                        "RB_TreeView::slotFilterOn() filter is empty or "
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

void RB_TreeView::slotFilterOff() {
    mModel->setWhere("");
    mToolButtonBar->setFilterOffEnabled(false);
}

void RB_TreeView::slotDemote() {
    QModelIndex idx = currentIndex();
    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                  "Please select an item first."));
        return;
    }

    QModelIndex sIdx = mModel->mapToSource(idx);
    RB_ObjectBase* obj = static_cast<RB_ObjectBase*>(sIdx.internalPointer());
    RB_ObjectContainer* sList = obj->getParent(); // sibling list

    DB_SelTreeSiblingDialog* dlg = new DB_SelTreeSiblingDialog(this);
    dlg->setSiblingList(sList);
    dlg->setHideId(obj->getId());
    dlg->init();
    int res = dlg->exec();

    if (res == QDialog::Accepted) {
        RB_String newParentId = dlg->getCurrentId();

        if (!newParentId.isEmpty()) {
            QModelIndex newIdx = mModel->demote(idx, newParentId);
            scrollTo(newIdx);
            setCurrentIndex(newIdx);
        }
    }

    dlg->deleteLater();
}

void RB_TreeView::slotPromote() {
    QModelIndex idx = currentIndex();
    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    idx = mModel->promote(idx);
    scrollTo(idx);
    setCurrentIndex(idx);
}

/**
 * \todo: to be tested
 * Move item up, only for internal memory models, proxy to be unsorted.
 * Gives unpredictable behaviour with database models
 * @brief RB_TreeView::slotUp
 */
void RB_TreeView::slotUp() {
    DB_DIALOGFACTORY->requestWarningDialog("Not yet implemented");
    //    if (!mModel) return;
    //
    //    QModelIndex index = trvParent->currentIndex();
    //
    //    // collapse sibling items first otherwise QTreeView shows open lines
    //    QModelIndex idxSibling;
    //    int parentRowCount = mModel->rowCount(index.parent());
    //    for (int row = 0; row < parentRowCount; ++row) {
    //        idxSibling = index.sibling(row, 0);
    //        trvParent->collapse(idxSibling);
    //    }
    //
    //    if (index.isValid() && index.row() > 0) {
    //        QModelIndex idxTo = mModel->index(index.row()-1, 0, index.parent());
    //        QModelIndex idxFrom = mModel->index(index.row(), 0, index.parent());
    //        mModel->swapObject(idxFrom, idxTo);
    //        trvParent->setCurrentIndex(idxTo);
    //    }
}

/**
 * \todo: to be tested
 * Move item down, only for internal memory models, proxy to be unsorted.
 * Gives unpredictable behaviour with database models
 * @brief RB_TreeView::slotDown
 */
void RB_TreeView::slotDown() {
    DB_DIALOGFACTORY->requestWarningDialog("Not yet implemented");
    //    if (!mModel) return;
    //
    //    QModelIndex index = trvParent->currentIndex();
    //
    //    // collapse sibling items first otherwise QTreeView shows open lines
    //    QModelIndex idxSibling;
    //    int parentRowCount = mModel->rowCount(index.parent());
    //    for (int row = 0; row < parentRowCount; ++row) {
    //        idxSibling = index.sibling(row, 0);
    //        trvParent->collapse(idxSibling);
    //    }
    //
    //    if (index.isValid() && index.row() < mModel->rowCount(index.parent()) - 1) {
    //        QModelIndex idxTo = mModel->index(index.row()+1, 0, index.parent());
    //        QModelIndex idxFrom = mModel->index(index.row(), 0, index.parent());
    //        mModel->swapObject(idxFrom, idxTo);
    //        trvParent->setCurrentIndex(idxFrom);
    //    }
}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 */
bool RB_TreeView::maybeSave() {
    if (mModel && mModel->isModelModified()) {
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

