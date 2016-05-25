/*****************************************************************
 * $Id: db_testtablewidget.cpp 1930 2013-04-30 20:21:24Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testtablewidget.h"

#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_testrelationdialog.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_tedelegate.h"

#include "modeltest.h"


DB_TestTableWidget::DB_TestTableWidget(QWidget *parent) : RB_Widget(parent) {
    RB_DEBUG->print("DB_TestTableWidget::DB_TestTableWidget()");
    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mComboBoxModel = NULL;
    mMapper = NULL;
}

DB_TestTableWidget::~DB_TestTableWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mComboBoxModel;
    delete mChildModel;
    delete mModel;

    RB_DEBUG->print("DB_TestTableWidget::~DB_TestTableWidget() OK");
}

void DB_TestTableWidget::init() {

    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTest);
    RB_String id = DB_MODELFACTORY->getRootId();
    mModel->setRoot(id);

//    RB_ObjectBase* root = mModel->getRoot();
//    RB_ObjectContainer* testList = root->getContainer("DB_TestList");

//    RB_ObjectBase* obj = NULL;

//    for (int i = 0; i < 3; ++i) {
//        obj = DB_TESTOBJECTFACTORY->newObject(RB_Uuid::createUuid(), testList);
//        obj->setValue("value1", "Test " + RB_String::number(i));
//    }

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leId, mModel->fieldIndex("id"));
    mMapper->addMapping(leParent, mModel->fieldIndex("parent"));
    mMapper->addMapping(leName, mModel->fieldIndex("name"));
    mMapper->addMapping(leValue1, mModel->fieldIndex("value1"));
    mMapper->addMapping(leValue2, mModel->fieldIndex("value2"));

    // use propertyName from QComboBox as "currentText" user properties
    // also possible, however currentIndex only is working correctly
    mMapper->addMapping(cbComboBox1, mModel->fieldIndex("combobox1"), "currentIndex");
    // create and attach fixed comboBox model
    RB_StringList items;
    items << tr("Home") << tr("Work") << tr("Other");
    mComboBoxModel = new QStringListModel(items, this);
    cbComboBox1->setModel(mComboBoxModel);
/*
    // Relational mapping, relation table should have a default row with id = "0"!
    int relationColumn = mModel->fieldIndex("combobox2");
    mModel->setRelation(relationColumn,
           QSqlRelation("DB_TestRelation", "id", "relation"));
    QSqlTableModel* relModel = mModel->relationModel(relationColumn);

    // Set data for in-memory relational mapping, testing only
    if (relModel) {
//        if (!mModel->database().isOpen()) {
//            relModel->insertRows(0, 1, QModelIndex());
//            relModel->insertRows(1, 1, QModelIndex());
//            relModel->insertRows(2, 1, QModelIndex());
//            QModelIndex index = relModel->index(0, 3, QModelIndex());
//            relModel->setData(index, "Home2",Qt::EditRole);
//            index = relModel->index(1, 3, QModelIndex());
//            relModel->setData(index, "Work2",Qt::EditRole);
//            index = relModel->index(2, 3, QModelIndex());
//            relModel->setData(index, "Other2",Qt::EditRole);
//        }

        cbComboBox2->setModel(relModel);
        int cbRelationColumn = relModel->fieldIndex("relation");
        if (!mModel->database().isOpen()) {
            // alternatively use a column number
            RB_MmAbstract* tmpRelModel = dynamic_cast<RB_MmAbstract*>(
                    mModel->relationModel(relationColumn));
            cbRelationColumn = tmpRelModel->fieldIndex("relation");
        }
        cbComboBox2->setModelColumn(cbRelationColumn);
        mMapper->setItemDelegate(new RB_SqlRelationalDelegate(mModel, this));
        mMapper->addMapping(cbComboBox2, relationColumn);
    }
*/
    addComboBoxMapping(mModel, "combobox2", "DB_TestRelation", "id",
                       "relation", cbComboBox2, mMapper);
    RB_String cbStyle = "QComboBox::drop-down {border-top-right-radius: 5px;"
                        "border-bottom-right-radius: 5px;}";
    cbComboBox2->setStyleSheet(cbStyle);

    // In case of no combobox mapping as above use:
//    int relationColumn = mModel->fieldIndex("combobox2");
//    mModel->setRelation(relationColumn,
//           QSqlRelation("DB_TestRelation", "id", "relation"));
    mMapper->addMapping(spSpinBox, mModel->fieldIndex("spinbox"));
    dteDateTimeEdit->setCalendarPopup(true);
    mMapper->addMapping(dteDateTimeEdit, mModel->fieldIndex("datetimeedit"));
    mMapper->addMapping(cbCheckBox, mModel->fieldIndex("checkbox"));
    mMapper->addMapping(leSelectRel, mModel->fieldIndex("selectrel_idx"));
    mMapper->addMapping(textEdit, mModel->fieldIndex("text"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
	// mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvParent, mModel);
    tvParent->setItemDelegateForColumn(mModel->fieldIndex("text"),
                                       new RB_TeDelegate(this));

    //
    // 1. Set child model
    // (2. Here no mapping)
    // (3. Root will be set by selecting row in parent model)
    // 4. connect to child (table) view
    //
    mChildModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestChild);
    setFormatTableView(tvChild, mChildModel);

    // TEST ONLY
    new ModelTest(mChildModel, this);

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool DB_TestTableWidget::fileSave(bool /*withSelect*/) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (mModel) {
        mModel->submitAllAndSelect();
        // HACK: Refresh relation model before selecting,
        // TODO: use signal slot between models to notify changes
        // NOTE: required if relational table has been modified
        //       and the source table is already queried.
        // QSqlTableModel* relModel = mModel->relationModel(mModel->fieldIndex("type_id"));
        // if (relModel) { // not for pipe line (yet)
        //     relModel->select();
        // }
    }
    if (mChildModel) {
        mChildModel->submitAllAndSelect();
    }

    setWindowModified(false);
    QApplication::restoreOverrideCursor();

    return true;
}

/**
 * Reverse all changes
 */
void DB_TestTableWidget::fileRevert() {
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
bool DB_TestTableWidget::closeWidget() {
    // Unregister widget
    RB_Widget::closeWidget();
    // Check for save data
    if (maybeSave()) {
        return true;
    }
    return false;
}

void DB_TestTableWidget::on_pbAdd_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    // insert at the end (or begining)
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("selectrel_idx"), QModelIndex());
    mModel->setData(index, "0", Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("value2"), QModelIndex());
//    mModel->setData(index, 0, Qt::EditRole);
    // end NOTE

    // mModel->submitAll();
    tvParent->setCurrentIndex(mModel->index(row, 0, QModelIndex()));
    tvParent->scrollTo(tvParent->currentIndex());
}

void DB_TestTableWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvParent->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAll();
    }
}

void DB_TestTableWidget::on_pbUp_clicked() {
    if (!mModel) return;

    QModelIndex index = tvParent->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QModelIndex idxTo = mModel->index(index.row()-1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tvParent->selectRow(index.row() - 1);
    }
}

void DB_TestTableWidget::on_pbDown_clicked() {
    if (!mModel) return;

    QModelIndex index = tvParent->currentIndex();
    if (index.isValid() && index.row() < mModel->rowCount(QModelIndex()) - 1) {
        QModelIndex idxTo = mModel->index(index.row()+1, 0, QModelIndex());
        QModelIndex idxFrom = mModel->index(index.row(), 0, QModelIndex());
        mModel->swapObject(idxFrom, idxTo);
        tvParent->selectRow(index.row() + 1);
    }
}

void DB_TestTableWidget::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTable(searchStr, tvParent);
}

void DB_TestTableWidget::on_pbFilter_clicked() {
    filterTable(tvParent);
}

void DB_TestTableWidget::on_pbFilterClear_clicked() {
    mModel->removeUserFilter();
}

void DB_TestTableWidget::on_pbSelectRel_clicked() {
    if (!tvParent->currentIndex().isValid()) return;

    // open selection dialog
    DB_TestRelationDialog* dlg = new DB_TestRelationDialog(this);
    dlg->init();


    // set data if item is selected
    if (dlg->exec() == QDialog::Accepted) {
//            QModelIndex idIdx = idx.model()->index(idx.row(), 0);
//            // RB_String str = idIdx.data().toString();
//            QModelIndex strIdx =  idx.model()->index(idx.row(), 3);
//            RB_String str = idIdx.data().toString() + strIdx.data().toString();



        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (!obj) {
            delete dlg;
            return;
        }

        RB_String id = obj->getId();
        RB_String val = obj->getValue(RB2::HIDDENCOLUMNS).toString();
        RB_String str = id + val;

        QModelIndex setIdx = mModel->index(tvParent->currentIndex().row(),
                                           mModel->fieldIndex("selectrel_idx"));
        mModel->setData(setIdx, str);
        mModel->submitAll();
    }

    delete dlg;
}

void DB_TestTableWidget::on_pbPrevious_clicked() {
    if (mMapper) mMapper->toPrevious();
}

void DB_TestTableWidget::on_pbNext_clicked() {
    if (mMapper) mMapper->toNext();
}

void DB_TestTableWidget::on_pbAddChild_clicked() {
    if (!mChildModel) return;

    mChildModel->undoFilterSort();

    QModelIndex index = tvParent->currentIndex();
    if (!index.isValid()) {
        return;
    }

    // always insert at the end
    int row = mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());

    tvChild->setCurrentIndex(mChildModel->index(row, 0, QModelIndex()));
    tvChild->scrollTo(tvChild->currentIndex());
}

void DB_TestTableWidget::on_pbDeleteChild_clicked() {
    if (!mChildModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvChild->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
    }
}

void DB_TestTableWidget::on_pbUpChild_clicked() {
    if (!mChildModel) return;

    QModelIndex index = tvChild->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QModelIndex idxTo = mChildModel->index(index.row()-1, 0, QModelIndex());
        QModelIndex idxFrom = mChildModel->index(index.row(), 0, QModelIndex());
        mChildModel->swapObject(idxFrom, idxTo);
        tvChild->selectRow(index.row() - 1);
    }
}

void DB_TestTableWidget::on_pbDownChild_clicked() {
    if (!mChildModel) return;

    QModelIndex index = tvChild->currentIndex();
    if (index.isValid() && index.row() < mChildModel->rowCount(QModelIndex()) - 1) {
        QModelIndex idxTo = mChildModel->index(index.row()+1, 0, QModelIndex());
        QModelIndex idxFrom = mChildModel->index(index.row(), 0, QModelIndex());
        mChildModel->swapObject(idxFrom, idxTo);
        tvChild->selectRow(index.row() + 1);
    }
}

void DB_TestTableWidget::on_pbTest_clicked() {
    QModelIndex index = tvParent->currentIndex();
    QModelIndex editIdx = mModel->index(index.row(), mModel->fieldIndex("combobox2"));
    RB_String str = mModel->data(editIdx, (Qt::UserRole + 1)).toString();
    textEdit->setText(str);
}

/**
 * to capture the close action by user at the right top corner
 */
void DB_TestTableWidget::reject() {
    RB_DEBUG->print("LDT_PedWidget::reject()");

    if (maybeSave()) {
        // QDialog::reject();
    }
}

bool DB_TestTableWidget::maybeSave() {
    if (isWindowModified()) {
        int ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(
                tr("Data changed ..."),
                tr("The data has been modified.\n"
                   "Do you want to keep your changes?"));
        if (ret == QMessageBox::Yes) {
            if (!fileSave(true)) { // if saved message by action
                DB_DIALOGFACTORY->statusBarMessage(tr("Data not saved"), 2000);
            } else {
                DB_DIALOGFACTORY->statusBarMessage(tr("Data saved"), 2000);
                // in fileSave() is setWindowModified(false);
            }
        } else if (ret == QMessageBox::No) {
            DB_DIALOGFACTORY->statusBarMessage(tr("Data not saved"), 2000);
            fileRevert(); // will setWindowModified(false)
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

void DB_TestTableWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
