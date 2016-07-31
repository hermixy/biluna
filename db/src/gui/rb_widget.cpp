/*****************************************************************
 * $Id: rb_widget.cpp 2233 2015-04-29 19:10:59Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_widget.h"

#include <QMessageBox>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "db_modelfactory.h"
#include "db_seltreesiblingdialog.h"
#include "rb_dialogfactory.h"
#include "rb_dialogwindow.h"
#include "rb_dockwidget.h"
#include "rb_mainwindow.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_sqlrelation.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_tableview.h"
#include "rb_textedit.h"
#include "rb_treeview.h"


/**
 * Constructor
 */
RB_Widget::RB_Widget(QWidget* parent, Qt::WindowFlags f)
        : QWidget(parent, f) {
    mWidgetType = -1;
    mWidgetId = "";
    mFullFileName = "";
    mDialogFactory = NULL;
    mRestartedAtBegin = false;
    mIsNewWidget = false;

    mAlternateBaseColor = QApplication::palette().color(QPalette::Active,
                                                        QPalette::AlternateBase);
    mBaseColor = QApplication::palette().color(QPalette::Active,
                                               QPalette::Base);
    mTextColor = QApplication::palette().color(QPalette::Active,
                                               QPalette::Text);
}

/**
 * Destructor, removes/deactivates widget in case of unique widget
 * at dialogFactory
 */
RB_Widget::~RB_Widget() {
    writeSettings();
    if (mDialogFactory) {
        mDialogFactory->removeWidget(mWidgetType);
    }

    RB_DEBUG->print("RB_Widget::~RB_Widget() OK");
}

/**
 * Get id of widget, applicable for non-unique widgets.
 * ID is used as name for non-unique widgets, for example the filename.
 * @return database document ID
 */
RB_String RB_Widget::getId() const {
    return mWidgetId;
}

/**
 * Set widget ID, for example new textwidget: 'Untitled2'
 * @param widgetId widget ID
 */
void RB_Widget::setId(const RB_String& widgetId) {
    mWidgetId = widgetId;
}

/**
 * Set document name to identify non-database (MDI) windows
 * @brief RB_Widget::setName
 * @param docName document name for example 'Untitled2' for new text document
 */
void RB_Widget::setName(const RB_String& docName) {
    mFullFileName = docName;
}

/**
 * Get proposed saveas filename. This is file path and name of current file or,
 * if empty, the window title (getName()) with space replaced by underscore
 * @returns fullfilename
 */
RB_String RB_Widget::getSaveAsFileName() {
    RB_String fn = mFullFileName;
    if (fn.isEmpty()) {
        fn = getName();
        fn = fn.replace(" ", "_");
    }
    return fn;
}

/**
 * @return int widget type
 */
int RB_Widget::getWidgetType() const {
    if (mWidgetType < 0) {
        RB_DEBUG->print("RB_Widget::getWidgetType() type not set ERROR");
    }
    return mWidgetType;
}

/**
 * Set widget type
 * @param type widget type
 */
void RB_Widget::setWidgetType(int type) {
    mWidgetType = type;
}

/**
 * Get parent widget type. The parent type is used by the MDI windows to
 * identify the corresponding dockwindows for creation and/or activation
 * @return parent widget type
 */
int RB_Widget::getParentWidgetType() const {
    return mParentWidgetType;
}

/**
 * Set parent widget type
 * @param type parent widget type
 */
void RB_Widget::setParentWidgetType(int type) {
    mParentWidgetType = type;
}

void RB_Widget::setDialogFactory(RB_DialogFactory* df) {
    mDialogFactory = df;
}

RB_DialogFactory *RB_Widget::getDialogFactory() {
    return mDialogFactory;
}

/**
 * Initialize widget internal state. Does nothing, override this function
 * Use to set widgets and model with possible child model(s)
 */
void RB_Widget::init() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("RB_Widget::init() not implemented"));
}

/**
 * Initialize widget focus. Does nothing, override this function
 * Use to set widgets and model with possible child model(s)
 */
void RB_Widget::initFocus() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("RB_Widget::initFocus() not implemented"));
}

/**
 * Set field to relational table and add combobox to mapper. Use also if only
 * a combobox alone without QTableView or QTreeView is to be mapped. If only
 * a relation is to be made in a QTableView or QTreeView use the first lines
 * of this function.
 * @param model current model with field to be mapped
 * @param field field to be mapped to relational table
 * @param relationTable relational table
 * @param relationId ID field of relational table and value that is in model
 * @param relationField field with value to be mapped and shown in model
 * @param comboBox combobox
 */
void RB_Widget::addComboBoxMapping(RB_MmProxy* model,
                                   const RB_String& field,
                                   const RB_String& relationTable,
                                   const RB_String& relationId,
                                   const RB_String& relationField,
                                   QComboBox* comboBox,
                                   RB_DataWidgetMapper* mapper) {
    // http://doc.qt.io/qt-5/qtsql-sqlwidgetmapper-example.html
    // Relational mapping, relation table should have a default row with id = "0"!

    int relationColumn = model->fieldIndex(field);
    model->setRelation(relationColumn,
                       RB_SqlRelation(relationTable, relationId, relationField));

    // Table or tree view combobox
    mapper->setItemDelegate(new RB_SqlRelationalDelegate(model, this));

    // Map combobox
    QSqlTableModel* relModel = model->relationModel(relationColumn);
    if (!relModel) return;

    comboBox->setModel(relModel);
    int cbRelationField = relModel->fieldIndex(relationField);

    if (!model->database().isOpen()) {
        // alternatively use a column number
        RB_MmAbstract* tmpRelModel = dynamic_cast<RB_MmAbstract*>(
                model->relationModel(relationColumn));
        cbRelationField = tmpRelModel->fieldIndex(relationField);
    }

    comboBox->setModelColumn(cbRelationField);
    mapper->addMapping(comboBox, relationColumn);

}

/**
 * Set data of a relationalmodel field, used when selecting an item from
 * a dialog with data to much for a combobox.
 * @param pm model to be set
 * @param field field name of the model
 * @param selIndex selected (visible) index of the relational selection source(!) model
 *   TODO: use consistent getProxyIndex or getCurrent(Source)Index()
 * @param le lineEdit to be updated
 */
void RB_Widget::setRelationalModelData(RB_MmProxy* pm,
                                       const RB_String& field,
                                       const QModelIndex& selIndex,
                                       QLineEdit* le) {
    if (!pm || field.isEmpty() || !selIndex.isValid()) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
                        "RB_Widget::setRelationalModelData() "
                        "invalid parameters WARNING");
        return;
    }

    // Get index to be set
    int row = pm->getProxyIndex().row(); // selIndex.row() is from other model
    int col = pm->fieldIndex(field);
    QModelIndex parent = pm->getProxyIndex().parent();
    QModelIndex setIdx = pm->index(row, col, parent);

    // Get relation
    RB_MmSource* sourceModel = dynamic_cast<RB_MmSource*>(pm->sourceModel());
    QSqlRelation rel = sourceModel->relation(col);
    const RB_MmProxy* lookUpModel = dynamic_cast<const RB_MmProxy*>(selIndex.model());

    // Set edit data (relational models do not have hidden columns)
    QModelIndex idIdx = lookUpModel->index(selIndex.row(),
                                           lookUpModel->fieldIndex(rel.indexColumn()),
                                           selIndex.parent());
    RB_Variant var = lookUpModel->hiddenData(idIdx, (Qt::UserRole + 1));
    pm->setData(setIdx, var, Qt::EditRole);

    // Set display data
    QModelIndex dispIdx = lookUpModel->index(selIndex.row(),
                                             lookUpModel->fieldIndex(rel.displayColumn()),
                                             selIndex.parent());
    var = lookUpModel->data(dispIdx, Qt::DisplayRole);
    pm->setData(setIdx, var, Qt::DisplayRole);
    // Hack for lineEdit:
    if (le) {
        le->setText(var.toString());
    }
}

/**
 * Set data of a relationalmodel field, used when selecting an item from
 * a dialog with data to much for a combobox.
 * @param pm model to be set
 * @param field field name of the model
 * @param selObj selected (visible) object of the relational model
 */
void RB_Widget::setRelationalModelData(RB_MmProxy* pm,
                                       const RB_String& field,
                                       RB_ObjectBase* selObj) {
    if (!pm || field.isEmpty() || !selObj) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
                        "RB_Widget::setRelationalModelData() "
                        "invalid parameters WARNING");
        return;
    } else if (!field.endsWith("_id")) {
        RB_DEBUG->error("RB_Widget::setRelationalModelData() "
                        + field +
                        "relational field does not end with _id ERROR");
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

    // Set display data. TODO: required?
//    var = selObj->getValue(rel.displayColumn());
//    pm->setData(setIdx, var, Qt::DisplayRole);
}

bool RB_Widget::isDuplicateIdxFound(RB_MmProxy* model,
                                    const RB_String& fieldName,
                                    const RB_String& id,
                                    int excludeRow) {
    if (!model) {
        return false;
    }

    QModelIndex iterIdx;
    int col = model->fieldIndex(fieldName);
    int rowCount = model->rowCount();
    bool found = false;

    for(int row = 0; row < rowCount && !found; row++) {
        iterIdx = model->index(row, col, QModelIndex());

        QString str = model->data(iterIdx, Qt::DisplayRole).toString();
        QRegExp regExp(id, Qt::CaseInsensitive, QRegExp::Wildcard);

        if (row != excludeRow && regExp.indexIn(str) >= 0) {
            found = true;
        }
    }

    return found;
}

bool RB_Widget::isDuplicateEntryFound(RB_MmProxy* model,
                                      const QString& fieldName,
                                      const QString& entry,
                                      int excludeRow) {
    if (!model) {
        return false;
    }

    QModelIndex iterIdx;
    int col = model->fieldIndex(fieldName);
    int rowCount = model->rowCount();
    bool found = false;

    for(int row = 0; row < rowCount && !found; row++) {
        iterIdx = model->index(row, col, QModelIndex());
        QString str = model->data(iterIdx, Qt::DisplayRole).toString();

        if (row != excludeRow && str == entry) {
            found = true;
        }
    }

    return found;
}

/**
 * Find text in table view
 * @param text search string
 * @param tv table view
 */
void RB_Widget::findInTable(const QString& text, QTableView* tv) {
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
        int ret = mDialogFactory->requestYesNoDialog(tr("Find"),
                      tr("Searched to end of document,\n"
                         "continue from the beginning?"));
        if (ret == QMessageBox::Yes) {
            mRestartedAtBegin = true;
            findInTable(text, tv);
        }
    } else if (!found) {
        mDialogFactory->requestWarningDialog("Finished searching.");
    }

    mRestartedAtBegin = false;
}

/**
 * Find text in tree view
 * @param text search string
 * @param trv tree view
 */
void RB_Widget::findInTree(const QString& text, QTreeView* trv) {
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
    } else {
        mDialogFactory->requestWarningDialog("No valid column selected.");
        return;
    }

    if (!found && !mRestartedAtBegin) {
        // start from beginning?
        int ret = mDialogFactory->requestYesNoDialog(tr("Find"),
                      tr("Searched column to end of document,\n"
                         "continue from the beginning?"));
        if (ret == QMessageBox::Yes) {
            mRestartedAtBegin = true;
            QModelIndex startIdx = m->index(0, currentIndex.column(), QModelIndex());
            trv->setCurrentIndex(startIdx);
            findInTree(text, trv);
        }
    } else if (!found) {
        mDialogFactory->requestWarningDialog("Finished column searching.");
    }

    mRestartedAtBegin = false;
}

/**
 * Filter table
 * @param tv table view
 */
void RB_Widget::filterTable(QTableView* tv) {
    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(tv->model());
    filter(m, tv);
}

///**
// * Filter tree, not working
// * @param trv tree view
// */
//void RB_Widget::filterTree(QTreeView* trv) {
//    RB_MmProxy* m = dynamic_cast<RB_MmProxy*>(trv->model());
//    QHeaderView* header = trv->header(); // not trv->horizontalHeader();
//    filter(m, header);
//}

/**
 * Set filter for the source model
 * @param m proxy model
 * @param tableName table name
 * @param fieldName field name
 * @param fStr filter string
 */
void RB_Widget::filterSourceModel(RB_MmProxy* m, const RB_String& tableName,
                                  const RB_String& fieldName,
                                  const RB_String& fStr) {
    if (fStr.isEmpty() || fStr.contains(" ")) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                        "RB_Widget::filterSourceModel() filter is empty or "
                        "has spaces INFO");
        mDialogFactory->commandMessage("No action: filter is empty or has spaces");
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
void RB_Widget::removeFilterSourceModel(RB_MmProxy* m) {
    m->setWhere("");
}

/**
 * Promote selected index of treeview. Takes first index,
 * if more indices are selected.
 * @param trv QTreeView
 */
void RB_Widget::promote(QTreeView* trv) {
    QModelIndex idx = trv->currentIndex();
    if (!idx.isValid()) {
        mDialogFactory->requestWarningDialog(tr("No item selected.\n"
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
void RB_Widget::demote(QTreeView* trv) {
    QModelIndex idx = trv->currentIndex();
    if (!idx.isValid()) {
        mDialogFactory->requestWarningDialog(tr("No item selected.\n"
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
void RB_Widget::keyPressEvent(QKeyEvent *e) {
    if(!(e->key() == Qt::Key_C && e->modifiers() & Qt::ControlModifier)) {
        QWidget::keyPressEvent(e);
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
        QWidget::keyPressEvent(e);
        return;
    }

    // Get selected indices of model
    QModelIndexList idxList;

    if (sm) {
        idxList = sm->selectedIndexes();
    } else {
        mDialogFactory->commandMessage("No selection model in RB_Widget::keyPressEvent()");
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

void RB_Widget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);

    switch (e->type()) {
    // Language change per each Ui widget
    //    case QEvent::LanguageChange:
    //        retranslateUi(this);
    //        break;
    case QEvent::PaletteChange:
        setPaletteColors();
        break;
    default:
        break;
    }
}

/**
 * Extra color members for text. Some custom reports have borders
 * that need to change color based on the global palette.
 * @brief RB_Widget::setPaletteColors
 */
void RB_Widget::setPaletteColors() {
    mAlternateBaseColor = QApplication::palette().color(QPalette::Active,
                                                        QPalette::AlternateBase);
    mBaseColor = QApplication::palette().color(QPalette::Active,
                                               QPalette::Base);
    mTextColor = QApplication::palette().color(QPalette::Active,
                                               QPalette::Text);
}

void RB_Widget::setIsNewWidget(bool isNewWidget) {
    mIsNewWidget = isNewWidget;
}

bool RB_Widget::isNewWidget() {
    return mIsNewWidget;
}

/**
 * Filter the model based on the selected header column
 * @param m model
 * @param header headerview
 */
void RB_Widget::filter(RB_MmProxy* m, QTableView* tv) {
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
 * Expand parent index, reentrant
 * @param idx index
 */
void RB_Widget::expandParent(QTreeView* trv, const QModelIndex& idx) {
    if (!trv || !idx.isValid()) return;

    if (!trv->isExpanded(idx.parent())) {
        expandParent(trv, idx.parent());
    }

    trv->expand(idx);
}

/**
 * File New.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::fileNew() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File New not implemented"));
}

/**
 * File Open, convenience function
 * not called by the main menu action DB_ActionFileOpen
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::fileOpen() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File Open not implemented"));
}

/**
 * File Save, called by the main menu action DB_ActionFileSave
 * To be overriden by actual widget, shows 'not implemented' message
 * @param withSelect true if also a select has to be done on the database
 */
bool RB_Widget::fileSave(bool /*withSelect*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File Save not implemented"));
    return true;
}

/**
 * File Save As, convenience function
 * not called by the main menu action DB_ActionFileSaveAs
 * To be overriden by actual widget, shows 'not implemented' message
 */
bool RB_Widget::fileSaveAs() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File Save As not implemented"));
    return true;
}

/**
 * File Revert, undo all changes to models
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::fileRevert() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("File Revert not implemented"));
}

/**
 * File export PDF.
 * To be overriden by actual widget, prints widget
 */
void RB_Widget::filePdf(QPrinter* pr) {
    printHelper(pr);
}

/**
 * File Print.
 * To be overriden by actual widget, prints widget
 */
void RB_Widget::filePrint(QPrinter* pr) {
    printHelper(pr);
}

/**
 * File Print Preview.
 * To be overriden by actual widget, prints widget
 */
void RB_Widget::filePrintPreview(QPrinter* pr) {
    printHelper(pr);
}

void RB_Widget::printHelper(QPrinter* pr) {
    QPainter p(pr);
    QPixmap pm = QPixmap::grabWidget(this);

    double widthRatio = pr->pageRect().width() / pm.width();
    double heightRatio = pr->pageRect().height() / pm.height();
    double scale = widthRatio < heightRatio ? widthRatio : heightRatio;
    QRect targetRect = QRect(0, 0,  pm.width() * scale,
                             pm.height() * scale);

    p.drawPixmap(targetRect, pm, QRect(0, 0, pm.width(), pm.height()));
}

/**
 * Edit Cut.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editCut() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Edit Cut not implemented"));
}

/**
 * Edit Copy.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editCopy() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Edit Copy not implemented"));
}

/**
 * Edit Paste.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editPaste() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Edit Paste not implemented"));
}

/**
 * Edit Undo.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editUndo() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Edit Undo not implemented"));
}

/**
 * Edit Redo.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editRedo() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Edit Redo not implemented"));
}

/**
 * Edit Select All.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editSelectAll() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Edit Select All not implemented"));
}

/**
 * @brief RB_Widget::editZoomIn
 * @param factor zoom factor or font increase (to Integer)
 */
void RB_Widget::editZoomIn(double /*factor*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Zoom In not implemented"));
}

/**
 * @brief RB_Widget::editZoomOut
 * @param factor zoom factor or font decrease (to Integer)
 */
void RB_Widget::editZoomOut(double /*factor*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Zoom Out not implemented"));
}

/**
 * Edit insert image.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editInsertImage(const QString& /* fn */) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Insert Image not implemented"));
}

/**
 * Edit create link.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::editCreateLink() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Create Link not implemented"));
}

/**
 * Format Bold
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatBold() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Bold not implemented"));
}

/**
 * Format Italic
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatItalic() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Italic not implemented"));
}

/**
 * Format Underline
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatUnderline() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Underline not implemented"));
}

/**
 * Format Underline
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatStrikethrough() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Strikethrough not implemented"));
}

/**
 * Format Align Left
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatAlignLeft() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Align Left not implemented"));
}

/**
 * Format Align Center
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatAlignCenter() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Align Center not implemented"));
}

/**
 * Format Align Right
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatAlignRight() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Align Right not implemented"));
}

/**
 * Format Align Justify
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatAlignJustify() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Align Jusitfy not implemented"));
}

/**
 * Format Increase Indent
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatIncreaseIndent() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Increase Indent not implemented"));
}

/**
 * Format Decrease Indent
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatDecreaseIndent() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Decrease Indent not implemented"));
}

/**
 * Format Numbered List
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatNumberedList() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Numbered List not implemented"));
}

/**
 * Format Bulleted List
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatBulletedList() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Bulleted List not implemented"));
}

/**
 * Format Font All
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatFontAll() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Font All not implemented"));
}

/**
 * Format Font Name
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatFontName() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Font Name not implemented"));
}

/**
 * Format Font Size
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatFontSize() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Font Size not implemented"));
}

/**
 * Format Text Color
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatTextColor() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Text Color not implemented"));
}

/**
 * Format Background Color
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatBackgroundColor() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Background Color not implemented"));
}


/**
 * Remove formatting and style
 * To be overriden by actual widget, shows 'not implemented' message
 */
void RB_Widget::formatRemove() {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Format Remove Style not implemented"));
}

/**
 * Get help subject, default the objectName() is returned.
 * @returns the help subject
 */
RB_String RB_Widget::getHelpSubject() const {
    return objectName();
}

/**
 * Load file, called by the main menu action DB_ActionFileOpen
 * To be overriden by actual widget, shows 'not implemented' message
 */
bool RB_Widget::loadFile(const RB_String&) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Load file not implemented"));
    return false;
}

/**
 * Save file, called by the main menu action DB_ActionFileSaveAs
 * To be overriden by actual widget, shows 'not implemented' message
 */
bool RB_Widget::saveFile(const RB_String&) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Save file not implemented"));
    return false;
}

/**
 * Set printer properties with paper size and margins
 * Does nothing, to be overriden by implementation.
 * Example code here.
 * @param pr printer
 */
void RB_Widget::setPrinterProperties(QPrinter* /*pr*/) {
    /*
    const bool oldFullPage = pr->fullPage();
    pr->setFullPage(true);
    // TODO: read from settings
    // pr.setPageSize(QPrinter::A4);
    // pr.setFullPage(true);
    // pr.setPageMargins(15, 10, 10, 10, QPrinter::Millimeter);
    pr->setPaperSize(mPageSize);
    pr->setOrientation(mOrientation);

    if (mPageMarginLeft > 0.0 || mPageMarginTop > 0.0
            || mPageMarginRight > 0.0 || mPageMarginBottom > 0.0) {
        pr->setPageMargins(mPageMarginLeft, mPageMarginTop, mPageMarginRight,
                          mPageMarginBottom, QPrinter::Millimeter);
    } else {
        pr->setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
    }

    pr->setFullPage(oldFullPage);
    */
}

/**
 * Set printer page margins, in millimeters
 * @param left
 * @param top
 * @param right
 * @param bottom
 */
//void RB_Widget::setPageMargins(double left, double top, double right, double bottom) {
//    mPageMarginLeft = left;
//    mPageMarginTop = top;
//    mPageMarginRight = right;
//    mPageMarginBottom = bottom;
//}

/**
 * Set combobox to lineedit style. Comboboxes are used
 * to show relational data but in this case the list is
 * (too) long for the dropdown function and a button next
 * to the combobox opens a selection dialog. This function
 * uses the setStyleSheet() function of the combobox to
 * hide the dropdown arrow and to disable the combobox
 * @param cb QComboBox
 */
void RB_Widget::setCbToLeStyle(QComboBox* cb) {
    cb->setStyleSheet(RB2::getCbToLeStyle());
    cb->setEnabled(false);
}

/**
 * Set standard format and connections of tableView and model
 * Do not use in combination with the RB_ToolButtonBar
 * @param tv tableView
 * @param m sortFilterProxyModel
 */
void RB_Widget::setFormatTableView(QTableView* tv, RB_MmProxy* m) {
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
 * Set standard format and connections of tableView and model.
 * Use when RB_ToolButtonBar is connected to tableView
 * @param tv tableView
 * @param m sortFilterProxyModel
 */
void RB_Widget::formatTableView(RB_TableView* tv, RB_MmProxy* m) {
    if (!tv) return;

    tv->setModel(m); // will create selection model

    if (!m) return;

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
 * Do not use in combination with the RB_ToolButtonBar
 * @param trv treeView
 * @param m sortFilterProxyModel
 */
void RB_Widget::setFormatTreeView(QTreeView* trv, RB_MmProxy* m) {
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
void RB_Widget::formatTreeView(RB_TreeView* trv, RB_MmProxy* m) {
    if (!trv) return;

    trv->setModel(m); // will create selection model

    if (!m) return;

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
 * Read widget size settings, always at end of init() method
 * of widget implementation
 */
void RB_Widget::readSettings() {
    if (!mDialogFactory || !parentWidget()) {
        RB_DEBUG->error("RB_Widget::readSettings() "
                        "mDialogFactory or parentWidget() NULL ERROR");
        return;
    }

    // Widget object name
    RB_SETTINGS->beginGroup(objectName());
    RB_DEBUG->print(objectName() + " - " + parentWidget()->objectName());

    if (parentWidget()->objectName() == "RB_DialogWindow") {
        QSize dlgSize = RB_SETTINGS->value(
                    "size", parentWidget()->sizeHint()).toSize();
        parentWidget()->resize(dlgSize);
    } else if (parentWidget()->objectName() == "RB_MdiWindow") {
        QMdiArea* mdiArea = mDialogFactory->getMainWindow()->getMdiArea();

        if (mdiArea && mdiArea->viewMode() != QMdiArea::TabbedView) {
            QPoint point = RB_SETTINGS->value("parentpos",
                                              QPoint(10, 10)).toPoint();
            QSize size = RB_SETTINGS->value("parentsize",
                                            parentWidget()->sizeHint()).toSize();

            if (point.x() < 0 || mdiArea->size().width() - 10 < point.x()
                    || point.y() < 0 || mdiArea->size().height() - 10 < point.y()) {
                // ensure visibility
                point.setX(10);
                point.setY(10);
            }

            parentWidget()->move(point);
            parentWidget()->resize(size);
        }

    }

    readChildrenSettings(this);
    RB_SETTINGS->endGroup();
}

/**
 * Read widget children bytearray settings, only for splitter
 * Recursive function
 * @param wObj widget
 */
void RB_Widget::readChildrenSettings(QWidget* wObj) {
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
                int width = RB_SETTINGS->value(tv->objectName() + "_col"
                                               + RB_String::number(col), -1).toInt();

                if (width > 0) {
                    tv->horizontalHeader()->resizeSection(col, width);
                }
            }
        } else if (trv) {
            // HACK: header not yet available
            int colCount = 20;

            for (int col = 0; col < colCount; ++col) {
                int width = RB_SETTINGS->value(trv->objectName() + "_col"
                                               + RB_String::number(col), -1).toInt();

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
 * Write widget size settings
 */
void RB_Widget::writeSettings() {
    if (!mDialogFactory || !parentWidget()) {
        RB_DEBUG->error("RB_Widget::writeSettings() "
                        "mDialogFactory or parentWidget() NULL ERROR");
        return;
    }

    // Widget object name
    RB_SETTINGS->beginGroup(objectName());
    RB_DEBUG->print(objectName() + " - " + parentWidget()->objectName());

    // HACK: cast comparison did not work, therefor objectname comparison
    if (parentWidget()->objectName() == "RB_DialogWindow") {
        RB_SETTINGS->setValue("size", parentWidget()->size());
    } else if (parentWidget()->objectName() == "RB_MdiWindow") {
        QMdiArea* mdiArea = mDialogFactory->getMainWindow()->getMdiArea();

        if (mdiArea && mdiArea->viewMode() != QMdiArea::TabbedView
                && !parentWidget()->isMinimized()) {
            RB_SETTINGS->setValue("parentpos", parentWidget()->pos());
            RB_SETTINGS->setValue("parentsize", parentWidget()->size());
        }
    } // Nothing yet on RB_DockWindow

    writeChildrenSettings(this);
    RB_SETTINGS->endGroup();
}

/**
 * Write widget children bytearray settings, only for splitter.
 * Recursive function
 * @param wObj widget
 */
void RB_Widget::writeChildrenSettings(QWidget* wObj) {
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
                    RB_SETTINGS->setValue(tv->objectName() + "_col"
                                          + RB_String::number(col),
                                          tv->horizontalHeader()
                                            ->sectionSize(col));
                }
            }
        } else if (trv) {
            int colCount = trv->header()->count();

            for (int col = 0; col < colCount; ++col) {
                if (!trv->header()->isSectionHidden(col)) {
                    RB_SETTINGS->setValue(trv->objectName() + "_col"
                                          + RB_String::number(col),
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
bool RB_Widget::maybeSave() {
    if (isWindowModified()) {
        int ret = mDialogFactory->requestYesNoCancelDialog(
                tr("Data changed ..."),
                tr("The data has been modified.\n"
                   "Do you want to keep your changes?"));
        if (ret == QMessageBox::Yes) {
            if (!fileSave(false)) {
                mDialogFactory->statusBarMessage(tr("Data not saved"), 2000);
            } else {
                mDialogFactory->statusBarMessage(tr("Data saved"), 2000);
                setWindowModified(false);
            }
        } else if (ret == QMessageBox::No) {
            // fileRevert(); used in RB_Dialog
            mDialogFactory->statusBarMessage(tr("Data not saved"), 2000);
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
void RB_Widget::beforeFileSave() {
    // HACK make sure to emit editingFinished() for current QLineEdit:
    focusNextChild();
}

/**
 * Close widget.
 * Called by parent MDI, dialog or dockwindow (top right corner)
 * close clicked. Override this function to handle unsaved
 * changes and/or settings with mayBeSave.
 * @returns true
 */
bool RB_Widget::closeWidget() {
    // Below is done in RB_Widget::~RB_Widget()
    // if (mDialogFactory) {
    //     mDialogFactory->removeWidget(getWidgetType());
    // }
    return maybeSave();
}

/**
 * Get current selected model index, to be implemented
 * in actual widget
 * @returns QModelIndex()
 */
QModelIndex RB_Widget::getCurrentModelIndex(int /*modelType*/) const {
    return QModelIndex();
}


/**
 * Size hint for this widget, default returns QWidget::sizeHint()
 * @return size
 */
QSize RB_Widget::sizeHint() const {
    if (mSizeHint.isValid()) {
        return mSizeHint;
    }
    return QWidget::sizeHint();
}

/**
 * @returns true if widget (data) has been changed
 */
bool RB_Widget::isWindowModified() const {
    if (parentWidget()) {
        // Normally this widget is the central widget of a MDI window
        return parentWidget()->isWindowModified();
    }
    return isWindowModified();
}

/**
 * Slot for filePrintPreview() dialog.
 * To be overriden by actual widget, shows 'not implemented' message
 * @pr printer
 */
void RB_Widget::slotPrintPreview(QPrinter* /*pr*/) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Slot Print Preview not implemented"));
}

/**
 * Transfer the signal dataIsChanged() from the model to the MDI window with
 * signal modificationChanged(true);
 * @param from from index, not used
 * @param to to index, not used
 */
void RB_Widget::slotDataIsChanged(const QModelIndex& /* from */,
                                  const QModelIndex& /* to */) {
    if (!parentWidget()) {
        // just in case, not used at the moment
        QWidget::setWindowModified(true);
    }
    emit modificationChanged(true);
}

/**
 * Transfer the signal dataIsSave() from the model to the MDI window with
 * signal modificationChanged(false);
 */
void RB_Widget::slotDataIsSaved() {
    if (!parentWidget()) {
        // just in case, not used at the moment
        QWidget::setWindowModified(false);
    }
    emit modificationChanged(false);
}

/**
 * Transfer the changed state from the widget to the MDI window
 * with signal modificationChanged();
 * @param changed true if modified
 */
void RB_Widget::setWindowModified(bool changed) {
    if (!parentWidget()) {
        // just in case, not used at the moment
        QWidget::setWindowModified(changed);
    }
    emit modificationChanged(changed);
}

/**
 * The header of a tableView or treeView has been clicked,
 * fetch all rows and set sorting to true.
 * @param idx modelIndex
 */
void RB_Widget::slotItemViewHeaderClicked(int logicalIndex) {
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

/**
 * Reset widget to default state such as set tabwidget index to 0
 * and set focus to first edit widget.
 * To be overriden, default does nothing.
 */
void RB_Widget::slotResetWidgets() {
    // does nothing
}

/**
 * @returns full file path and name
 */
RB_String RB_Widget::getCurrentFileName() {
    return mFullFileName;
}

/**
 * Set current file name
 */
void RB_Widget::setCurrentFileName(const RB_String& fileName) {
    mFullFileName = QFileInfo(fileName).canonicalFilePath();
}

