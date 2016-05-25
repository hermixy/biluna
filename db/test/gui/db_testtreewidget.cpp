/*****************************************************************
 * $Id: db_testtreewidget.cpp 1446 2011-08-02 20:23:23Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testtreewidget.h"

#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_tedelegate.h"

/**
 * Constructor
 */
DB_TestTreeWidget::DB_TestTreeWidget(QWidget* parent) : RB_Widget(parent) {
    RB_DEBUG->print("DB_TestTreeWidget::DB_TestTreeWidget()");
    setupUi(this);
    mModel = NULL;
    mChildModel = NULL;
    mComboBoxModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
DB_TestTreeWidget::~DB_TestTreeWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mComboBoxModel;
    delete mChildModel;
    delete mModel;
    // delete mMapper; if model is deleted also the mapper is deleted otherwise include
    RB_DEBUG->print("DB_TestTableWidget::~DB_TestTableWidget() OK");
}

void DB_TestTreeWidget::init() {
    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestTree);
    RB_String id = DB_MODELFACTORY->getRootId();
    mModel->setRoot(id);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(treeLeId,  mModel->fieldIndex("id"));
    mMapper->addMapping(treeLeParent, mModel->fieldIndex("parent"));
    mMapper->addMapping(treeLeName, mModel->fieldIndex("name"));
    mMapper->addMapping(treeLeValue1, mModel->fieldIndex("value1"));
    mMapper->addMapping(treeLeValue2, mModel->fieldIndex("value2"));

    // use propertyName from QComboBox as "currentText" user properties
    // also possible, however currentIndex only is working correctly
    mMapper->addMapping(treeCbComboBox1, mModel->fieldIndex("combobox1"), "currentIndex");
    // create and attach fixed comboBox model
    RB_StringList items;
    items << tr("Home") << tr("Work") << tr("Other");
    mComboBoxModel = new QStringListModel(items, this);
    treeCbComboBox1->setModel(mComboBoxModel);

    // Relational mapping, relation table should have a default row with id = "0"!
//    int relationColumn = mModel->fieldIndex("combobox2");
//    mModel->setRelation(relationColumn,
//           QSqlRelation("DB_TestRelation", "id", "relation"));
/*    QSqlTableModel* relModel = mModel->relationModel(relationColumn);

    // Set data for in-memory relational mapping, testing only
    if (relModel) {
//        if (!mModel->database().isOpen()) {
//            relModel->insertRows(0, 1, QModelIndex());
//            relModel->insertRows(1, 1, QModelIndex());
//            relModel->insertRows(2, 1, QModelIndex());
//            QModelIndex index = relModel->index(0, 3, QModelIndex());
//            relModel->setData(index, "Home3",Qt::EditRole);
//            index = relModel->index(1, 3, QModelIndex());
//            relModel->setData(index, "Work3",Qt::EditRole);
//            index = relModel->index(2, 3, QModelIndex());
//            relModel->setData(index, "Other3",Qt::EditRole);
//        }

        treeCbComboBox2->setModel(relModel);
        int cbRelationColumn = relModel->fieldIndex("relation");
        if (!mModel->database().isOpen()) {
            // alternatively use a column number
            RB_MmAbstract* tmpRelModel = dynamic_cast<RB_MmAbstract*>(
                    mModel->relationModel(relationColumn));
            cbRelationColumn = tmpRelModel->fieldIndex("relation");
        }
        treeCbComboBox2->setModelColumn(cbRelationColumn);
        mMapper->setItemDelegate(new RB_SqlRelationalDelegate(mModel, this));
        mMapper->addMapping(treeCbComboBox2, relationColumn);
    }
*/
    addComboBoxMapping(mModel, "combobox2", "DB_TestRelation", "id",
                       "relation", treeCbComboBox2, mMapper);

    mMapper->addMapping(leTrvCombo2, mModel->fieldIndex("combobox2"));
    mMapper->addMapping(treeSpSpinBox, mModel->fieldIndex("spinbox"));
    treeDteDateTimeEdit->setCalendarPopup(true);
    mMapper->addMapping(treeDteDateTimeEdit, mModel->fieldIndex("datetimeedit"));
    mMapper->addMapping(treeCbCheckBox, mModel->fieldIndex("checkbox"));
    mMapper->addMapping(textEdit, mModel->fieldIndex("text"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    mModel->setHeaderData(RB2::HIDDENCOLUMNS, Qt::Horizontal, tr("Rutger"));

    //
    // 4. Connect model to main view
    //
    setFormatTreeView(trvParent, mModel);
//    trvParent->setItemDelegate(new DB_TeBlockDelegate(this));
    trvParent->setItemDelegateForColumn(mModel->fieldIndex("text"),
                                        new RB_TeDelegate(this));

    //
    // 1. Set child model
    // (2. Here no mapping)
    // (3. Root will be set by selecting row in parent model)
    // 4. connect to child (table) view
    //
    mChildModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestTreeChild);
    setFormatTableView(tvTableChild, mChildModel);


    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());
    readSettings();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool DB_TestTreeWidget::fileSave() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
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
void DB_TestTreeWidget::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
    if (mChildModel) {
        mChildModel->revertAll();
    }
    setWindowModified(false);
}

/**
 * Edit Cut.
 */
void DB_TestTreeWidget::editCut() {
    textEdit->cut();
}

/**
 * Edit Copy.
 */
void DB_TestTreeWidget::editCopy() {
    textEdit->copy();
}

/**
 * Edit Paste.
 */
void DB_TestTreeWidget::editPaste() {
    textEdit->paste();
}

/**
 * Edit Undo.
 */
void DB_TestTreeWidget::editUndo() {
    textEdit->undo();
}

/**
 * Edit Redo.
 */
void DB_TestTreeWidget::editRedo() {
    textEdit->redo();
}

/**
 * Format Bold
 */
void DB_TestTreeWidget::formatBold() {
    textEdit->slotTextBold();
}

/**
 * Format Italic
 */
void DB_TestTreeWidget::formatItalic() {
    textEdit->slotTextItalic();
}

/**
 * Format Underline
 */
void DB_TestTreeWidget::formatUnderline() {
    textEdit->slotTextUnderline();
}

/**
 * Format Strikethrough
 */
void DB_TestTreeWidget::formatStrikethrough() {
    textEdit->slotTextStrikeOut();
}

/**
 * Format Align Left
 */
void DB_TestTreeWidget::formatAlignLeft() {
    textEdit->slotTextAlign(RB_TextEdit::AlignLeft);
}

/**
 * Format Align Center
 */
void DB_TestTreeWidget::formatAlignCenter() {
    textEdit->slotTextAlign(RB_TextEdit::AlignCenter);
}

/**
 * Format Align Right
 */
void DB_TestTreeWidget::formatAlignRight() {
    textEdit->slotTextAlign(RB_TextEdit::AlignRight);
}

/**
 * Format Align Justify
 */
void DB_TestTreeWidget::formatAlignJustify() {
    textEdit->slotTextAlign(RB_TextEdit::AlignJustify);
}

/**
 * Format Increase Indent
 */
void DB_TestTreeWidget::formatIncreaseIndent() {
    textEdit->slotTextIndent(1);
}

/**
 * Format Decrease Indent
 */
void DB_TestTreeWidget::formatDecreaseIndent() {
    textEdit->slotTextIndent(-1);
}

/**
 * Format numbered list
 */
void DB_TestTreeWidget::formatNumberedList() {
    textEdit->slotTextStyle(RB_TextEdit::ListDecimal);
}

/**
 * Format bulleted list
 */
void DB_TestTreeWidget::formatBulletedList() {
    textEdit->slotTextStyle(RB_TextEdit::ListDisc);
}

/**
 * Format font family
 */
void DB_TestTreeWidget::formatFontName() {
    textEdit->slotTextFamily();
}

/**
 * Format font size
 */
void DB_TestTreeWidget::formatFontSize() {
    textEdit->slotTextSize();
}

/**
 * Format text color
 */
void DB_TestTreeWidget::formatTextColor() {
    textEdit->slotTextColor();
}

/**
 * Format background color
 */
void DB_TestTreeWidget::formatBackgroundColor() {
    textEdit->slotBackgroundColor();
}

/**
 * Remove formatting and style
 */
void DB_TestTreeWidget::formatRemove() {
//    webView->triggerPageAction(QWebPage::RemoveFormat);
}

/**
 * Close widget, also called by right top corner close icon.
 * Unregisters widget from dialogfactory and via maybeSave calls fileSave.
 */
bool DB_TestTreeWidget::closeWidget() {
    // Unregister widget
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
void DB_TestTreeWidget::on_pbTreeAdd_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvParent->currentIndex().isValid()) {
        idxParent = trvParent->currentIndex().parent();
    }

    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mModel->index(row, mModel->fieldIndex("selectrel_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    // mModel->submitAll();
    trvParent->setCurrentIndex(mModel->index(row, 0, idxParent));
    trvParent->scrollTo(trvParent->currentIndex());
}


/**
 * Add child item (at sub level of selected item)
 */
void DB_TestTreeWidget::on_pbTreeAddChild_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    QModelIndex idxParent = QModelIndex();
    if (trvParent->currentIndex().isValid()) {
        idxParent = trvParent->currentIndex();
    }
    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mModel->index(row, mModel->fieldIndex("selectrel_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    trvParent->setCurrentIndex(mModel->index(row, 0, idxParent));
    trvParent->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, idxParent));
}

void DB_TestTreeWidget::on_pbTreeDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = trvParent->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
        mModel->submitAll();
    }
}

//void DB_TestTreeWidget::on_pbTreeUp_clicked() {
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
//void DB_TestTreeWidget::on_pbTreeDown_clicked() {
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

void DB_TestTreeWidget::on_pbFind_clicked() {
    QString searchStr = leFind->text();
    findInTree(searchStr, trvParent);
}

//void DB_TestTreeWidget::on_pbFilter_clicked() {
////    filterTree(trvParent);
//    DB_DIALOGFACTORY->requestWarningDialog("Filtering not possible in tree view.");
//}
//
//void DB_TestTreeWidget::on_pbFilterClear_clicked() {
////    mModel->removeUserFilter();
//    DB_DIALOGFACTORY->requestWarningDialog("Filtering not possible in tree view.");
//}

void DB_TestTreeWidget::on_pbTableAddChild_clicked() {
    if (!mChildModel) return;

    mChildModel->undoFilterSort();

    // always insert at the end
    int row = mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());
    tvTableChild->setCurrentIndex(mChildModel->index(row, 0, QModelIndex()));
    tvTableChild->scrollTo(tvTableChild->currentIndex());
}

void DB_TestTreeWidget::on_pbTableDeleteChild_clicked() {
    if (!mChildModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvTableChild->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
    }
}

void DB_TestTreeWidget::on_pbTableUpChild_clicked() {
    if (!mChildModel) return;

    QModelIndex index = this->tvTableChild->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QModelIndex idxTo = mChildModel->index(index.row()-1, 0, index.parent());
        QModelIndex idxFrom = mChildModel->index(index.row(), 0, index.parent());
        mChildModel->swapObject(idxFrom, idxTo);
        tvTableChild->selectRow(index.row() - 1);
    }

}

void DB_TestTreeWidget::on_pbTableDownChild_clicked() {
    if (!mChildModel) return;

    QModelIndex index = tvTableChild->currentIndex();
    if (index.isValid() && index.row() < mChildModel->rowCount(index.parent()) - 1) {
        QModelIndex idxTo = mChildModel->index(index.row()+1, 0, index.parent());
        QModelIndex idxFrom = mChildModel->index(index.row(), 0, index.parent());
        mChildModel->swapObject(idxFrom, idxTo);
        tvTableChild->selectRow(index.row() + 1);
    }
}

/**
 * Debug message or information
 */
void DB_TestTreeWidget::on_pbTest_clicked() {
/*
    int count = mModel->rowCount();
    RB_String str = "ProxyModelRowCount = " + RB_String::number(count);
    count = mModel->sourceModel()->rowCount();
    str += "\nSourceModelRowCount = " + RB_String::number(count);
    RB_String test = mModel->data(mModel->index(1, mModel->fieldIndex("value1"), QModelIndex()),Qt::DisplayRole).toString();
    str += "\nValue1 (row 1) = " + test;
    DB_DIALOGFACTORY->requestWarningDialog(str);
*/

    textEdit->slotTextBold();
}

void DB_TestTreeWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
