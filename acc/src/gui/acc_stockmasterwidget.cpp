/*****************************************************************
 * $Id: acc_stockmasterwidget.cpp 2165 2014-09-24 19:22:36Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockmasterwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelation.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_StockMasterWidget::ACC_StockMasterWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
ACC_StockMasterWidget::~ACC_StockMasterWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel;
    delete mChildModel;
    RB_DEBUG->print("ACC_StockMasterWidget::~ACC_StockMasterWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_StockMasterWidget::init() {
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockCategory);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    // relations
//    mModel->setRelation(mModel->fieldIndex("stockact_idx"),
//           RB_SqlRelation("ACC_ChartMaster", "id", "accountcode"));
//    mModel->setRelation(mModel->fieldIndex("adjglact_idx"),
//           RB_SqlRelation("ACC_ChartMaster", "id", "accountcode"));
//    mModel->setRelation(mModel->fieldIndex("purchpricevaract_idx"),
//           RB_SqlRelation("ACC_ChartMaster", "id", "accountcode"));
//    mModel->setRelation(mModel->fieldIndex("materialuseagevarac_idx"),
//           RB_SqlRelation("ACC_ChartMaster", "id", "accountcode"));
//    mModel->setRelation(mModel->fieldIndex("wipact_idx"),
//           RB_SqlRelation("ACC_ChartMaster", "id", "accountcode"));
    mModel->setRelation(mModel->fieldIndex("taxcat_id"),
           RB_SqlRelation("ACC_TaxCategory", "id", "taxcatname"));

    // also with treemodel
    mModel->select();

    setFormatTreeView(trvStockCategory, mModel);
    trvStockCategory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockMaster);

    mChildModel->setRelation(mChildModel->fieldIndex("unitofmeasurement"),
           RB_SqlRelation("ACC_UnitOfMeasure", "id", "unitname"));

    setFormatTableView(tvStockMaster, mChildModel);
    tvStockMaster->setEditTriggers(QAbstractItemView::NoEditTriggers);
    readSettings();

    // Hidden columns only for tableviews, now treeview
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i >= 2) {
            trvStockCategory->hideColumn(i);
        } else {
            trvStockCategory->showColumn(i);

            if (trvStockCategory->columnWidth(i) < 5) {
                trvStockCategory->setColumnWidth(i, 100);
            }
        }
    }

    // Hidden columns only for tableviews
    colCount = mChildModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvStockMaster->hideColumn(i);
        } else {
            tvStockMaster->showColumn(i);

            if (tvStockMaster->columnWidth(i) < 5) {
                tvStockMaster->setColumnWidth(i, 100);
            }
        }
    }

    QList<int> list;
    list << 200 << 400;
    splitter->setSizes(list);
}

/**
 * Save data to disk
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_StockMasterWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();

    if (withSelect) {
        if (mChildModel->submitAllAndSelect() && mModel->submitAllAndSelect()) {
            
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else {
        if (mChildModel->submitAll() && mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }

    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add clicked
 */
void ACC_StockMasterWidget::on_pbAddCategory_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    // Make sure corresponding detail MDI window is open for maybeSave()
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetStockCategory);
    mdiWin->show();

    QModelIndex idxParent = QModelIndex();
    if (trvStockCategory->currentIndex().isValid()) {
        idxParent = trvStockCategory->currentIndex().parent();
    }

    int row = 0; // mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mModel->index(row, mModel->fieldIndex("categorycode"), idxParent);
    mModel->setData(index, "<NEW>", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("stockact_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("adjglact_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("purchpricevaract_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("materialuseagevarac_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("wipact_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    mModel->submitAll(); // for a treemodel does not invalidate the index
    // end NOTE

    trvStockCategory->setCurrentIndex(mModel->index(row, 0, idxParent));
    trvStockCategory->scrollTo(trvStockCategory->currentIndex());
    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Button add sub (category) clicked
 */
void ACC_StockMasterWidget::on_pbSubCategory_clicked() {
    if (!mModel) return;

    mModel->undoFilterSort();

    // Make sure corresponding detail MDI window is open for maybeSave()
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetStockCategory);
    mdiWin->show();
    ACC_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    QModelIndex idxParent = QModelIndex();
    if (trvStockCategory->currentIndex().isValid()) {
        idxParent = trvStockCategory->currentIndex();
    } else {
        on_pbAddCategory_clicked();
        return;
    }
    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table (in tableView otherwise new row will not show)
    QModelIndex index = mModel->index(row, mModel->fieldIndex("categorycode"), idxParent);
    mModel->setData(index, "<NEW>", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("stockact_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("adjglact_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("purchpricevaract_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("materialuseagevarac_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("wipact_idx"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
    mModel->submitAll(); // for a treemodel does not invalidate the index
    // end NOTE

    trvStockCategory->setCurrentIndex(mModel->index(row, 0, idxParent));
    trvStockCategory->scrollTo(mModel->index(row, 0, idxParent));
    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Button delete clicked
 */
void ACC_StockMasterWidget::on_pbDeleteCategory_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = trvStockCategory->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button save clicked
 */
void ACC_StockMasterWidget::on_pbSave_clicked() {
    fileSave(true);
}

void ACC_StockMasterWidget::on_pbPromote_clicked() {
    promote(trvStockCategory);
}

void ACC_StockMasterWidget::on_pbDemote_clicked() {
    demote(trvStockCategory);
}

/**
 * Button add clicked
 */
void ACC_StockMasterWidget::on_pbAddItem_clicked() {
    if (!mChildModel) return;

    mChildModel->undoFilterSort();

    // Make sure corresponding detail MDI window is open for maybeSave()
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetStockItem);
    mdiWin->show();
    ACC_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    // always insert at the beginning
    int row = 0; // mChildModel->rowCount();
    mChildModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mChildModel->index(row, mChildModel->fieldIndex("stockcode"), QModelIndex());
    mChildModel->setData(index, "<NEW>", Qt::EditRole);
    index = mChildModel->index(row, mChildModel->fieldIndex("unitofmeasurement"), QModelIndex());
    mChildModel->setData(index, "0", Qt::EditRole);
//    index = mChildModel->index(row, mChildModel->fieldIndex("taxcat_id"), QModelIndex());
//    mChildModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvStockMaster->setCurrentIndex(mChildModel->index(row, RB2::HIDDENCOLUMNS));
    tvStockMaster->scrollTo(tvStockMaster->currentIndex());
    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Button delete clicked
 */
void ACC_StockMasterWidget::on_pbDeleteItem_clicked() {
    if (!mChildModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvStockMaster->currentIndex();
    if (index.isValid()) {
        mChildModel->removeRows(index.row(), 1, QModelIndex());
        mChildModel->submitAllAndSelect();
        // no setModified(false) here because parent could have changed
    }
}

/**
 * Set source model filter
 */
void ACC_StockMasterWidget::on_pbFilterOn_clicked() {
    filterSourceModel(mChildModel, "acc_stockmaster", "stockcode", leFilter->text());
}

/**
 * Remove source model filter
 */
void ACC_StockMasterWidget::on_pbFilterOff_clicked() {
    leFilter->setText("");
    removeFilterSourceModel(mChildModel);
}


bool ACC_StockMasterWidget::closeWidget() {
    return RB_Widget::closeWidget();
}

/**
 * @returns current selected model index
 */
QModelIndex ACC_StockMasterWidget::getCurrentModelIndex(int modelType) const {
    if (modelType == ACC_ModelFactory::ModelStockCategory) {
        return trvStockCategory->currentIndex();
    } else if (modelType == ACC_ModelFactory::ModelStockMaster) {
        return tvStockMaster->currentIndex();
    }
    return QModelIndex();
}

/**
 * Change event, for example translation
 */
void ACC_StockMasterWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
