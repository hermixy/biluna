/*****************************************************************
 * $Id: db_testtabletreewidget.cpp 1421 2011-05-16 20:49:06Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testtabletreewidget.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_tedelegate.h"

/**
 * Constructor
 */
DB_TestTableTreeWidget::DB_TestTableTreeWidget(QWidget* parent) : RB_Widget(parent) {
    RB_DEBUG->print("DB_TestTableTreeWidget::DB_TestTableTreeWidget()");
    setupUi(this);
    mModel = NULL;
    mChildModel = NULL;
    mComboBoxModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
DB_TestTableTreeWidget::~DB_TestTableTreeWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mComboBoxModel;
    delete mChildModel;
    delete mModel;
    // delete mMapper; if model is deleted also the mapper is deleted otherwise include
    RB_DEBUG->print("DB_TestTableWidget::~DB_TestTableWidget() OK");
}

void DB_TestTableTreeWidget::init() {
    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTvTrv);
    RB_String id = DB_MODELFACTORY->getTestRootId();
    mModel->setRoot(id);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    // -> In this test widget part of childmodel

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvParent, mModel);
    tvParent->setItemDelegateForColumn(mModel->fieldIndex("text"),
                                        new RB_TeDelegate(this));

    //
    // 1. Set child model
    mChildModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTvTrvChild);

    // 2. Mapping
    mMapper = mChildModel->getMapper();
    mMapper->addMapping(leId,  mChildModel->fieldIndex("id"));
    mMapper->addMapping(leParent, mChildModel->fieldIndex("parent"));
    mMapper->addMapping(leName, mChildModel->fieldIndex("name"));
    mMapper->addMapping(leNumber, mChildModel->fieldIndex("number"));
    mMapper->addMapping(leRevision, mChildModel->fieldIndex("revision"));
    mMapper->addMapping(leDescription, mChildModel->fieldIndex("description"));
    mMapper->addMapping(leCompany, mChildModel->fieldIndex("company"));
    mMapper->addMapping(leLocation, mChildModel->fieldIndex("location"));

    // (3. Root will be set by selecting row in parent model)
    //    mChildModel->setRoot("None");
    //    mChildModel->select();

    // 4. Connect model to child (table) view
    //
    mChildModel->select(); // only for childtree model
    setFormatTreeView(trvChild, mChildModel);

    // Hide columns treeview
//    for (int i = 0; i < mChildModel->columnCount(QModelIndex()); ++i) {
//        if (i < RB2::HIDDENCOLUMNS) {
//            trvChild->hideColumn(i);
//        }
//    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool DB_TestTableTreeWidget::fileSave() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (mModel) {
        mModel->submitAll();
    }
    if (mChildModel) {
        mChildModel->submitAll();
    }

    setWindowModified(false);
    QApplication::restoreOverrideCursor();

    return true;
}

/**
 * Reverse all changes
 */
void DB_TestTableTreeWidget::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
    if (mChildModel) {
        mChildModel->revertAll();
    }
    setWindowModified(false);
}

/**
 * Close widget, also called by right top corner close icon.
 * Unregisters widget from dialogfactory and via maybeSave calls fileSave.
 */
bool DB_TestTableTreeWidget::closeWidget() {
    // Unregsiter widget
    RB_Widget::closeWidget();
    // Check for save data
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Add sibling item (at same level as selected item)
 */
void DB_TestTableTreeWidget::on_pbTableAdd_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
//    if (trvParent->currentIndex().isValid()) {
//        idxParent = trvParent->currentIndex().parent();
//    }

    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mModel->index(row, mModel->fieldIndex("selectrel_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    // mModel->submitAll();
    tvParent->setCurrentIndex(mModel->index(row, 0, idxParent));
    tvParent->scrollTo(tvParent->currentIndex());
}


void DB_TestTableTreeWidget::on_pbTableDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvParent->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
		mModel->submitAll();
    }
}

//void DB_TestTableTreeWidget::on_pbTreeUp_clicked() {
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
//}
//
//void DB_TestTableTreeWidget::on_pbTreeDown_clicked() {
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
//
//}

void DB_TestTableTreeWidget::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTable(searchStr, tvParent);
}

//void DB_TestTableTreeWidget::on_pbFilter_clicked() {
////    filterTree(trvParent);
//    DB_DIALOGFACTORY->requestWarningDialog("Filtering not possible in tree view.");
//}
//
//void DB_TestTableTreeWidget::on_pbFilterClear_clicked() {
////    mModel->removeUserFilter();
//    DB_DIALOGFACTORY->requestWarningDialog("Filtering not possible in tree view.");
//}

void DB_TestTableTreeWidget::on_pbTreeAdd_clicked() {
    if (!mChildModel) return;

    if (!tvParent->currentIndex().isValid()) {
        return;
    }

    mChildModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvChild->currentIndex().isValid()) {
        idxParent = trvChild->currentIndex().parent();
    }
    int row = mChildModel->rowCount();
    mChildModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mChildModel->index(row, mChildModel->fieldIndex("selectrel_idx"), idxParent);
    mChildModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    trvChild->setCurrentIndex(mChildModel->index(row, 0, QModelIndex())); // 0 for treeview
    trvChild->scrollTo(trvChild->currentIndex());
}

/**
 * Add child item (at sub level of selected item)
 */
void DB_TestTableTreeWidget::on_pbTreeAddChild_clicked() {
    if (!mChildModel) return;

    if (!tvParent->currentIndex().isValid()) {
        return;
    }

    mChildModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvChild->currentIndex().isValid()) {
        idxParent = trvChild->currentIndex();
    }
    int row = mChildModel->rowCount(idxParent);
    mChildModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mChildModel->index(row, mChildModel->fieldIndex("selectrel_idx"), idxParent);
    mChildModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    trvChild->setCurrentIndex(mChildModel->index(row, 0, idxParent)); // 0 for treeview
    trvChild->scrollTo(trvChild->currentIndex());
}

void DB_TestTableTreeWidget::on_pbTreeDelete_clicked() {
    if (!mChildModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = trvChild->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, index.parent());
		mChildModel->submitAll();
    }
}

void DB_TestTableTreeWidget::on_pbPromote_clicked() {
    if (!mChildModel) return;

//    QModelIndex index = this->tvTableChild->currentIndex();
//    if (index.isValid() && index.row() > 0) {
//        QModelIndex idxTo = mChildModel->index(index.row()-1, 0, index.parent());
//        QModelIndex idxFrom = mChildModel->index(index.row(), 0, index.parent());
//        mChildModel->swapObject(idxFrom, idxTo);
//        tvTableChild->selectRow(index.row() - 1);
//    }

}

void DB_TestTableTreeWidget::on_pbDemote_clicked() {
    if (!mChildModel) return;

//    QModelIndex index = tvTableChild->currentIndex();
//    if (index.isValid() && index.row() < mChildModel->rowCount(index.parent()) - 1) {
//        QModelIndex idxTo = mChildModel->index(index.row()+1, 0, index.parent());
//        QModelIndex idxFrom = mChildModel->index(index.row(), 0, index.parent());
//        mChildModel->swapObject(idxFrom, idxTo);
//        tvTableChild->selectRow(index.row() + 1);
//    }
}

/**
 * Debug message or information
 */
void DB_TestTableTreeWidget::on_pbTest_clicked() {
    int count = mModel->rowCount();
    RB_String str = "ProxyModelRowCount = " + RB_String::number(count);
    count = mModel->sourceModel()->rowCount();
    str += "\nSourceModelRowCount = " + RB_String::number(count);
    RB_String test = mModel->data(mModel->index(1, mModel->fieldIndex("value1"), QModelIndex()),Qt::DisplayRole).toString();
    str += "\nValue1 (row 1) = " + test;
    DB_DIALOGFACTORY->requestWarningDialog(str);
}

void DB_TestTableTreeWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
