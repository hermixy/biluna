/*****************************************************************
 * $Id: rb_dialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 28, 2010 3:21:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_dialog.h"

#include <QComboBox>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_settings.h"
#include "rb_sqlrelation.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_tableview.h"
#include "rb_treeview.h"


/**
 * Constructor
 */
RB_Dialog::RB_Dialog(QWidget* parent) : QDialog(parent) { //, Qt::Tool) {
    mRestartedAtBegin = false;
    setWindowIcon(QIcon(":/images/biluna16.png"));
    // setWindowModality(Qt::ApplicationModal);
}

/**
 * Destructor
 */
RB_Dialog::~RB_Dialog() {
    writeSettings();
}

RB_String RB_Dialog::getSaveAsFileName() {
    return RB_String();
}

/**
 * Initialize base widget. Does nothing, override this function
 * Use to set widgets and model with possible child model(s)
 */
void RB_Dialog::init() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("init() not implemented"));
}

/**
 * File Save.
 * To be overriden by actual dialog, shows 'not implemented' message
 * @param withSelect true if select() is to be executed after submitAll()
 */
bool RB_Dialog::fileSave(bool /*withSelect*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File Save not implemented"));
    return false;
}

/**
 * File Revert.
 * To be overriden by actual dialog, shows 'not implemented' message
 */
void RB_Dialog::fileRevert() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File Revert not implemented"));
}

/**
 * @brief RB_Dialog::filePdf
 * @param pr printer
 */
void RB_Dialog::filePdf(QPrinter* /*pr*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File PDF not implemented"));
}

/**
 * @brief RB_Dialog::filePrint
 * @param pr printer
 */
void RB_Dialog::filePrint(QPrinter* /*pr*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File print not implemented"));
}

/**
 * @brief RB_Dialog::setPrinterProperties
 * @param pr printer
 */
void RB_Dialog::setPrinterProperties(QPrinter* /*pr*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Set printer properties not implemented"));
}

QString RB_Dialog::getCurrentFileName() {
    return mFullFileName;
}

void RB_Dialog::setCurrentFileName(const QString& fileName) {
    mFullFileName = fileName;
}

/**
 * Set field to relational table and add combobox to mapper. Use also if only
 * a combobox alone without QTableView or QTreeView is to be mapped. If only
 * a relation is to be made in a QTableView or QTreeView use the first lines
 * of this function. Same function as in RB_Widget(Interface).
 * @param model current model with field to be mapped
 * @param field field to be mapped to relational table
 * @param relationTable relational table
 * @param relationId ID field of relational table and value that is in model
 * @param relationField field with value to be mapped and shown in model
 * @param comboBox combobox
 * @param relWhere where clause for relation table
 */
void RB_Dialog::addComboBoxMapping(RB_MmProxy* model,
                                   const RB_String& field,
                                   const RB_String& relationTable,
                                   const RB_String& relationId,
                                   const RB_String& relationField,
                                   QComboBox* comboBox,
                                   RB_DataWidgetMapper* mapper) {
    // Relational mapping, relation table should have a default row with id = "0"!
    int relationColumn = model->fieldIndex(field);
    model->setRelation(relationColumn,
                       RB_SqlRelation(relationTable, relationId, relationField));

    // Map combobox
    QSqlTableModel* relModel = model->relationModel(relationColumn);
    if (!relModel) return;

    comboBox->setModel(relModel);
    int cbRelationColumn = relModel->fieldIndex(relationField);

    if (!model->database().isOpen()) {
        // alternatively use a column number
        RB_MmAbstract* tmpRelModel = dynamic_cast<RB_MmAbstract*>(
                model->relationModel(relationColumn));
        cbRelationColumn = tmpRelModel->fieldIndex(relationField);
    }

    comboBox->setModelColumn(cbRelationColumn);

    // Table or tree view combobox
    if (!mapper->itemDelegate()) {
        mapper->setItemDelegate(new RB_SqlRelationalDelegate(model, this));
    }
    mapper->addMapping(comboBox, relationColumn);
}

/**
 * Set data of a relationalmodel field, used when selecting an item from
 * a dialog with data to much for a combobox.
 * @param pm model to be set
 * @param field field name of the model
 * @param selObj selected (visible) object of the relational model
 */
void RB_Dialog::setRelationalModelData(RB_MmProxy* pm,
                                       const RB_String& field,
                                       RB_ObjectBase* selObj) {
    if (!pm || field.isEmpty() || !selObj) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
                        "RB_Dialog::setRelationalModelData() "
                        "invalid parameters WARNING");
        return;
    } else if (!field.endsWith("_id")) {
        RB_DEBUG->error("RB_Dialog::setRelationalModelData() "
                        + field +
                        " relational field does not end with _id ERROR");
    }

    // Get index to be set
    int row = pm->getProxyIndex().row();
    int col = pm->fieldIndex(field);
    QModelIndex parent = pm->getProxyIndex().parent();
    QModelIndex setIdx = pm->index(row, col, parent);

    // Get relation
    RB_MmSource* sourceModel = dynamic_cast<RB_MmSource*>(pm->sourceModel());
    QSqlRelation rel = sourceModel->relation(col);

    // Set edit data (relational models do not have hidden columns)
    RB_Variant var = selObj->getValue(rel.indexColumn()); // usually id
    pm->setData(setIdx, var, Qt::EditRole);

    // Set display data, TODO: required?
//    var = selObj->getValue(rel.displayColumn());
//    pm->setData(setIdx, var, Qt::DisplayRole);

}

/**
 * Set combobox to lineedit style. Comboboxes are used
 * to show relational data but in this case the list is
 * (too) long for the dropdown function and a button next
 * to the combobox opens a selection dialog. This function
 * uses the setStyleSheet() function of the combobox to
 * hide the dropdown arrow and to disable the combobox
 * @param cb QComboBox
 */
void RB_Dialog::setCbToLeStyle(QComboBox* cb) {
    cb->setStyleSheet(RB2::getCbToLeStyle());
    cb->setEnabled(false);
}


///**
// * On button Help clicked
// */
//void RB_Dialog::on_pbHelp_clicked() {
//    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
//    mw->slotHelpSubject(this->objectName());
//}

///**
// * On button OK clicked
// */
//void RB_Dialog::on_pbOk_clicked() {
//    fileSave();
//    accept();
//}

///**
// * On button Cancel clicked
// */
//void RB_Dialog::on_pbCancel_clicked() {
//    reject();
//}

/**
 * Capture the close action by user at the right top corner. In a mainwindow
 * the closeEvent(event) is used.
 */
void RB_Dialog::reject() {
    if (maybeSave()) {
        QDialog::reject();
    }
}

/**
 * @brief RB_Dialog::slotPrintPreview
 * @param pr
 */
void RB_Dialog::slotPrintPreview(QPrinter* /*pr*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Slot print preview not implemented"));
}

/**
 * Transfer the signal dataChanged() from the model to the dialog with
 * signal modificationChanged(true);
 * @param from from index not used
 * @param to to index not used
 */
void RB_Dialog::slotDataIsChanged(const QModelIndex& /* from */,
                                  const QModelIndex& /* to */) {
    setWindowModified(true);
}

/**
 * Transfer the signal dataIsSave() from the model to the MDI window with
 * signal modificationChanged(false);
 */
void RB_Dialog::slotDataIsSaved() {
    setWindowModified(false);
}

/**
 * Set standard format and connections of tableView and model
 * @param tv tableView
 * @param m sortFilterProxyModel
 */
void RB_Dialog::setFormatTableView(QTableView* tv, RB_MmProxy* m) {
    if (!tv || !m) return;

    tv->setModel(m); // will create selection model
    m->setSelectionModel(tv->selectionModel());

    tv->setAlternatingRowColors(true);
    tv->verticalHeader()->setDefaultSectionSize(20); // row height

    tv->setEditTriggers(QAbstractItemView::EditKeyPressed |
                        QAbstractItemView::DoubleClicked);
    // trv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->setItemDelegate(new RB_SqlRelationalDelegate(m, this));

    // To change row in mapper and emit row change for possible child models
    connect(tv->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            m, SLOT(slotChangeCurrentRow(const QModelIndex&, const QModelIndex&)));
    // To set windowModified is true
    connect(m, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(m, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));
    // To set sorting to true and load all rows
    connect(tv->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(slotItemViewHeaderClicked(int)));
}

/**
 * Set standard format and connections of tableView and model.
 * Use when RB_ToolButtonBar is connected to tableView
 * @param tv tableView
 * @param m sortFilterProxyModel
 */
void RB_Dialog::formatTableView(RB_TableView* tv, RB_MmProxy* m) {
    if (!tv || !m) return;

    tv->setModel(m); // will create selection model
    m->setSelectionModel(tv->selectionModel());

    tv->setAlternatingRowColors(true);
    tv->verticalHeader()->setDefaultSectionSize(20); // row height

    tv->setEditTriggers(QAbstractItemView::EditKeyPressed |
                        QAbstractItemView::DoubleClicked);
    // tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->setItemDelegate(new RB_SqlRelationalDelegate(m, this));

    // To change row in mapper and emit row change for possible child models
    connect(tv->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            m, SLOT(slotChangeCurrentRow(const QModelIndex&, const QModelIndex&)));
    // To set windowModified is true or false
    connect(m, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(m, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));
    connect(m, SIGNAL(modelReverted()),
            this, SLOT(slotDataIsSaved()));
    // To set sorting to true and load all rows
    connect(tv->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(slotItemViewHeaderClicked(int)));
}

/**
 * Set standard format and connections of treeView and model
 * @param trv treeView
 * @param m sortFilterProxyModel
 */
void RB_Dialog::setFormatTreeView(QTreeView* trv, RB_MmProxy* m) {
    if (!trv || !m) return;

    trv->setModel(m); // will create selection model
    m->setSelectionModel(trv->selectionModel());

    trv->setAlternatingRowColors(true);
    // trv->verticalHeader()->setDefaultSectionSize(20); // row height
    trv->setUniformRowHeights(true); // for optimization

    trv->setEditTriggers(QAbstractItemView::EditKeyPressed |
                         QAbstractItemView::DoubleClicked);
    // trv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    trv->setItemDelegate(new RB_SqlRelationalDelegate(m, this));

    // To change row in mapper and emit row change for possible child models
    connect(trv->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            m, SLOT(slotChangeCurrentRow(const QModelIndex&, const QModelIndex&)));
    // To set windowModified is true
    connect(m, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(m, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));
    // Expand tree for tree models
    connect(trv, SIGNAL(expanded(const QModelIndex&)),
            m, SLOT(slotExpanded(const QModelIndex&)));
    // To set sorting to true and load all rows
    connect(trv->header(), SIGNAL(sectionClicked(int)),
            this, SLOT(slotItemViewHeaderClicked(int)));
}

/**
 * Set standard format and connections of treeView and model.
 * Use when RB_ToolButtonBar is connected to treeView
 * @param trv treeView
 * @param m sortFilterProxyModel
 */
void RB_Dialog::formatTreeView(RB_TreeView* trv, RB_MmProxy* m) {
    if (!trv || !m) return;

    trv->setModel(m); // will create selection model
    m->setSelectionModel(trv->selectionModel());

    trv->setAlternatingRowColors(true);
    // trv->verticalHeader()->setDefaultSectionSize(20); // row height
    trv->setUniformRowHeights(true); // for optimization

    trv->setEditTriggers(QAbstractItemView::EditKeyPressed |
                         QAbstractItemView::DoubleClicked);
    // trv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    trv->setItemDelegate(new RB_SqlRelationalDelegate(m, this));

    // To change row in mapper and emit row change for possible child models
    connect(trv->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            m, SLOT(slotChangeCurrentRow(const QModelIndex&, const QModelIndex&)));
    // To set windowModified is true
    connect(m, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(m, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));
    // Expand tree for tree models
    connect(trv, SIGNAL(expanded(const QModelIndex&)),
            m, SLOT(slotExpanded(const QModelIndex&)));
    // To set sorting to true and load all rows
    connect(trv->header(), SIGNAL(sectionClicked(int)),
            this, SLOT(slotItemViewHeaderClicked(int)));
}

/**
 * Read dialog size settings (position has been removed)
 */
void RB_Dialog::readSettings() {
//    QPoint pnt = QPoint(100,100);
//    if (parentWidget()) {
//        pnt = parentWidget()->pos() + QPoint(20, 20);
//    }
    RB_SETTINGS->beginGroup(objectName());
//    QPoint pos = RB_SETTINGS->value("pos", pnt).toPoint();
    QSize size = RB_SETTINGS->value("size", sizeHint()).toSize();
    readChildrenSettings(this);
    RB_SETTINGS->endGroup();

    resize(size);
//    move(pos);
}

/**
 * Read widget children bytearray settings, only for splitter
 * Recursive function
 * @param wObj widget
 */
void RB_Dialog::readChildrenSettings(QWidget* wObj) {
    if (!wObj) return;

    const QObjectList& wdgtList = wObj->children();
    int count = wdgtList.size();

    for (int i = 0; i < count; ++i) {
        QObject* obj = wdgtList.at(i);
        QSplitter* spl = qobject_cast<QSplitter*>(obj);
        QTableView* tv = qobject_cast<QTableView*>(obj);
        QTreeView* trv = qobject_cast<QTreeView*>(obj);

        if (spl) {
            QByteArray state = RB_SETTINGS->value(spl->objectName(), "").toByteArray();
            spl->restoreState(state);
        } else if (tv) {
            // HACK: header not yet available
            int colCount = 20;

            for (int col = 0; col < colCount; ++col) {
                int width = RB_SETTINGS->value(tv->objectName() + "_col" + RB_String::number(col), -1).toInt();

                if (width > 0) {
                    tv->horizontalHeader()->resizeSection(col, width);
                }
            }
        } else if (trv) {
            // HACK: header not yet available
            int colCount = 20;

            for (int col = 0; col < colCount; ++col) {
                int width = RB_SETTINGS->value(trv->objectName() + "_col" + RB_String::number(col), -1).toInt();

                if (width > 0) {
                    trv->header()->resizeSection(col, width);
                }
            }
        }

        QWidget* wdgt = qobject_cast<QWidget*>(obj);

        if (wdgt) {
            readChildrenSettings(wdgt);
        }
    }
}

/**
 * Write dialog size settings (position has been removed)
 */
void RB_Dialog::writeSettings() {
    RB_SETTINGS->beginGroup(objectName());
//    RB_SETTINGS->setValue("pos", pos());
    RB_SETTINGS->setValue("size", size());
    writeChildrenSettings(this);
    RB_SETTINGS->endGroup();
}

/**
 * Write widget children bytearray settings, only for splitter.
 * Recursive function
 * @param wObj widget
 */
void RB_Dialog::writeChildrenSettings(QWidget* wObj) {
    if (!wObj) return;

    const QObjectList& wdgtList = wObj->children();
    int count = wdgtList.size();

    for (int i = 0; i < count; ++i) {
        QObject* obj = wdgtList.at(i);
        QSplitter* spl = qobject_cast<QSplitter*>(obj);
        QTableView* tv = qobject_cast<QTableView*>(obj);
        QTreeView* trv = qobject_cast<QTreeView*>(obj);

        if (spl) {
            RB_SETTINGS->setValue(spl->objectName(), spl->saveState());
        } else if (tv) {
            int colCount = tv->horizontalHeader()->count();

            for (int col = 0; col < colCount; ++col) {
                if (!tv->horizontalHeader()->isSectionHidden(col)) {
                    RB_SETTINGS->setValue(tv->objectName() + "_col" + RB_String::number(col),
                                          tv->horizontalHeader()->sectionSize(col));
                }
            }
        } else if (trv) {
            int colCount = trv->header()->count();

            for (int col = 0; col < colCount; ++col) {
                if (!trv->header()->isSectionHidden(col)) {
                    RB_SETTINGS->setValue(trv->objectName() + "_col" + RB_String::number(col),
                                          trv->header()->sectionSize(col));
                }
            }
        }

        QWidget* wdgt = qobject_cast<QWidget*>(obj);

        if (wdgt) {
            writeChildrenSettings(wdgt);
        }
    }
}



/**
 * Notify user of unsaved data and ask whether data should be saved.
 */
bool RB_Dialog::maybeSave() {
    beforeFileSave();

    if (isWindowModified()) {
        int ret = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Data changed ..."),
                tr("The data has been modified.\n"
                   "Do you want to keep your changes?"));
        if (ret == QMessageBox::Yes) {
            if (!fileSave(false)) {
                DB_DIALOGFACTORY->statusBarMessage(tr("Data not saved"), 2000);
            } else {
                DB_DIALOGFACTORY->statusBarMessage(tr("Data saved"), 2000);
                setWindowModified(false);
            }
        } else if (ret == QMessageBox::No) {
            fileRevert();
            DB_DIALOGFACTORY->statusBarMessage(tr("Data not saved"), 2000);
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

/**
 * HACK: Before fileSave() the data of the current QLineEdit
 * needs to be send to the database by emitting editingFinished().
 * This is handled in this function by setting the focus to the
 * next child.
 */
void RB_Dialog::beforeFileSave() {
    // HACK make sure to emit editingFinished() for current QLineEdit:
    focusNextChild();
}

/**
 * Close event of dialog, event cannot be rejected
 * @param e event
 */
void RB_Dialog::closeEvent(QCloseEvent* e) {
    if (isWindowModified()) {
        if (maybeSave()) {
            e->accept();
        } else {
            e->ignore();
        }
    }
    e->accept();
}

/**
 * Find text in table view
 * @param text search string
 * @param tv table view
 */
void RB_Dialog::findInTable(const QString& text, QTableView* tv) {
    if (text.isEmpty()) return;
    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(tv->model());
    if (!m) return;

    QModelIndex currentIndex = tv->currentIndex();
    QModelIndex nextIndex = m->index(0, 0, QModelIndex());
    int rowCount = tv->verticalHeader()->count();
    int colCount = tv->horizontalHeader()->count();

    if (currentIndex.isValid()
            && currentIndex.row() <= rowCount - 1
            && currentIndex.column() <= colCount - 1
            && !mRestartedAtBegin) {
        // continue search
        if (currentIndex.column() == colCount - 1
            && currentIndex.row() < rowCount - 1) {
            // before last row on last column
            nextIndex = m->index(currentIndex.row() + 1, 0, QModelIndex());
        } else if (currentIndex.column() < colCount - 1) {
            // valid row on before last column
            nextIndex = m->index(currentIndex.row(),
                                      currentIndex.column() + 1, QModelIndex());
        }
    }

    // m->match(...) refer findInTree

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
                nextIndex = m->index(row, col, QModelIndex());
            }

            QString str = m->data(nextIndex, Qt::DisplayRole).toString();
            QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);

            if (regExp.indexIn(str) >= 0) {
                // found
                currentIndex = nextIndex;
                tv->setCurrentIndex(currentIndex);
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
            findInTable(text, tv);
        }
    } else if (!found) {
        DB_DIALOGFACTORY->requestWarningDialog("Finished searching.");
    }

    mRestartedAtBegin = false;
}

/**
 * Find text in tree view
 * @param text search string
 * @param trv tree view
 */
void RB_Dialog::findInTree(const QString& text, QTreeView* trv) {
    if (text.isEmpty()) return;
    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(trv->model());
    if (!m) return;

    QModelIndex currentIndex = trv->currentIndex();
    bool found = false;

    if (currentIndex.isValid()) {
        QModelIndex idx;
        QModelIndexList idxList;
        int count = 1;

        while (!found && count != 0) {
            idxList = m->match(currentIndex, Qt::DisplayRole, text, 2,
                               Qt::MatchContains | Qt::MatchRecursive);
            count = idxList.count();

            if (count > 0) {
                idx = idxList.first();

                if (idx != currentIndex) {
                    // found current item
                    currentIndex = idx;

                    if (!trv->isColumnHidden(idx.column())) {
                        expandParent(trv, idx);
                        trv->setCurrentIndex(idx);
                        found = true;
                    }
                } else if (count > 1) {
                    // found another item
                    idx = idxList.at(1);
                    currentIndex = idx;

                    if (!trv->isColumnHidden(idx.column())) {
                        expandParent(trv, idx);
                        trv->setCurrentIndex(idx);
                        found = true;
                    }
                } else {
                    // found only current item
                    count = 0;
                }
            }
        }
    }

    if (!found && !mRestartedAtBegin) {
        // start from beginning?
        int ret = DB_DIALOGFACTORY->requestYesNoDialog(tr("Find"),
                      tr("Searched column to end of document,\n"
                         "continue from the beginning?"));
        if (ret == QMessageBox::Yes) {
            mRestartedAtBegin = true;
            QModelIndex startIdx = m->index(0, currentIndex.column(), QModelIndex());
            trv->setCurrentIndex(startIdx);
            findInTree(text, trv);
        }
    } else if (!found) {
        DB_DIALOGFACTORY->requestWarningDialog("Finished column searching.");
    }

    mRestartedAtBegin = false;
}

/**
 * Filter table
 * @param tv table view
 */
void RB_Dialog::filterTable(QTableView* tv) {
    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(tv->model());
    filter(m, tv);
}

/**
 * Fetch all data if available, not all databases support this feature
 * @param m proxymodel
 * @param parent parent modelindex
 */
//void RB_Dialog::fetchAll(RB_MmProxy* m, const QModelIndex& parent) {
//    if (!m) return;

//    if (m->canFetchMore(parent)) {
//        m->fetchMore(parent);
//        fetchAll(m, parent);
//    }
//}

/**
 * Set filter for the source model
 * @param m proxy model
 * @param tableName table name
 * @param fieldName field name
 * @param fStr filter string
 */
void RB_Dialog::filterSourceModel(RB_MmProxy* m, const RB_String& tableName,
                                  const RB_String& fieldName,
                                  const RB_String& fStr) {
    if (fStr.isEmpty() || fStr.contains(" ")) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                        "RB_Dialog::filterSourceModel() filter is empty or "
                        "has spaces INFO");
        DB_DIALOGFACTORY->commandMessage("No action: filter is empty or has spaces");
        return;
    }

    RB_String filter = "`" + tableName + "`.`" + fieldName + "` LIKE '";
    filter += fStr;

    if (!fStr.endsWith("%")) {
        filter += "%";
    }

    filter += "'";
    m->setWhere(filter);
}

/**
 * Remove filter of the source model
 * @param m proxy model
 */
void RB_Dialog::removeFilterSourceModel(RB_MmProxy* m) {
    m->setWhere("");
}

/**
 * Promote selected index of treeview. Takes first index,
 * if more indices are selected.
 * @param trv QTreeView
 */
void RB_Dialog::promote(QTreeView* trv) {
    QModelIndex idx = trv->currentIndex();
    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(trv->model());
    idx = m->promote(idx);

    trv->scrollTo(idx);
    trv->setCurrentIndex(idx);
}

/**
 * Demote selected index of treeview, will open a dialog
 * for the new parent selection out of the siblings of
 * the selected item. Takes first index if more indices
 * are selected.
 * @param trv QTreeView
 */
void RB_Dialog::demote(QTreeView* trv) {
    QModelIndex idx = trv->currentIndex();
    if (!idx.isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(trv->model());
    QModelIndex sIdx = m->mapToSource(idx);
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
            QModelIndex newIdx = m->demote(idx, newParentId);
            trv->scrollTo(newIdx);
            trv->setCurrentIndex(newIdx);
        }
    }

    dlg->deleteLater();
}

/**
 * At CTRL+C event copy data to the global clipboard
 * only takes the surrounding square selection
 */
void RB_Dialog::keyPressEvent(QKeyEvent *e) {
    if(!(e->key() == Qt::Key_C && e->modifiers() & Qt::ControlModifier)) {
        QDialog::keyPressEvent(e);
        return;
    }

    // Find out whether a tableView has focus
    QItemSelectionModel* sm = NULL;
    QAbstractItemModel* m = NULL;
    QList<QAbstractItemView*> vwList = this->findChildren<QAbstractItemView*>();

    foreach (QAbstractItemView* vw, vwList) {
        if (vw && vw->hasFocus()) {
            sm = vw->selectionModel();
            m = vw->model();
        }
    }

    if (!sm || !m) {
        QDialog::keyPressEvent(e);
        return;
    }

    // Get selected indices of model
    QModelIndexList idxList;

    if (sm) {
        idxList = sm->selectedIndexes();
    } else {
        DB_DIALOGFACTORY->commandMessage("No selection model in RB_Dialog::keyPressEvent()");
        return;
    }

    // Create rectangle of selected indices
    int minRowNo = 1000000000; // 10+e09
    int maxRowNo = 0;
    int minColNo = 1000000000; // 10+e09
    int maxColNo = 0;

    QModelIndex idx;

    foreach(idx, idxList) {
        if (idx.row() < minRowNo) {
            minRowNo = idx.row();
        }
        if (idx.row() > maxRowNo) {
            maxRowNo = idx.row();
        }
        if (idx.column() < minColNo) {
            minColNo = idx.column();
        }
        if (idx.column() > maxColNo) {
            maxColNo = idx.column();
        }
    }

    // Create tab separated list of text only
    QByteArray byteArray;

    for (int row = minRowNo; row <= maxRowNo; ++row) {
        for (int col = minColNo; col <= maxColNo; ++col) {
            idx = m->index(row, col);
            byteArray.append(idx.data().toString() + "\t");
        }
        byteArray.append("\n");
    }

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("text/plain", byteArray);
    QApplication::clipboard()->setMimeData(mimeData);

    e->accept();
}

///**
// * Filter tree
// * @param trv tree view
// */
//void RB_Dialog::filterTree(QTreeView* trv) {
//    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(trv->model());
//    QHeaderView* header = trv->header(); // not trv->horizontalHeader();
//    filter(m, header);
//}

void RB_Dialog::filter(RB_MmProxy* m, QTableView* tv) {
    if (!m || !tv) return;
    int count = m->columnCount();

    // fill combobox with available column names
    DB_FilterDialog* dlg = new DB_FilterDialog(this);
    for (int i = RB2::HIDDENCOLUMNS; i < count; i++) {
        if (!tv->isColumnHidden(i)) {
            dlg->addFieldName(m->headerData(i, Qt::Horizontal).toString());
        }
    }
    dlg->addFieldName(tr("All"));

    // set current filter text
    QRegExp regExp = m->getUserFilterExpression();
    if (regExp.isValid()) {
        dlg->setFieldName(m->getUserFilterFieldName());
        dlg->setFilterText(regExp.pattern());
        if (Qt::CaseSensitive == regExp.caseSensitivity()) {
            dlg->setMatchCase(true);
        } else {
            dlg->setMatchCase(false);
        }
    } else {
        dlg->setFieldName("");
        dlg->setFilterText("");
        dlg->setMatchCase(false);
    }

    int ret = dlg->exec();

    if (ret == QDialog::Accepted) {
        // set filter
        m->setUserFilter(dlg->getFilterText(),
                         dlg->getFieldName(),
                         dlg->getMatchCase());
    }

    dlg->deleteLater();
}

/**
 * Expand parent index, reintrant
 * @param idx index
 */
void RB_Dialog::expandParent(QTreeView* trv, const QModelIndex& idx) {
    if (!trv || !idx.isValid()) return;

    if (!trv->isExpanded(idx.parent())) {
        expandParent(trv, idx.parent());
    }

    trv->expand(idx);
}

/**
 * The header of a tableView or treeView has been clicked,
 * fetch all rows and set sorting to true.
 * @param idx modelIndex
 */
void RB_Dialog::slotItemViewHeaderClicked(int logicalIndex) {
    QHeaderView* hv = qobject_cast<QHeaderView*>(sender());
    if (!hv) return;
    RB_MmProxy* m = qobject_cast<RB_MmProxy*>(hv->model());
    if (!m) return;

    QModelIndex idx = QModelIndex();
    QTableView* tv = NULL;
    tv = qobject_cast<QTableView*>(hv->parentWidget());

    if (tv && !tv->isSortingEnabled()) {
        m->fetchAll();
        tv->sortByColumn(logicalIndex);
        tv->setSortingEnabled(true);
        tv->clearSelection();
        return;
    } else if (tv) {
        idx = tv->currentIndex();

        if (idx.isValid()) {
            tv->scrollTo(idx);
        }

        return;
    }

    QTreeView* trv = NULL;
    trv = qobject_cast<QTreeView*>(hv->parentWidget());

    if (trv && !trv->isSortingEnabled()) {
        m->fetchAll();
        trv->sortByColumn(logicalIndex);
        trv->setSortingEnabled(true);
        trv->clearSelection();
    } else if (trv) {
        idx = trv->currentIndex();

        if (idx.isValid()) {
            trv->scrollTo(idx);
        }
    }
}
